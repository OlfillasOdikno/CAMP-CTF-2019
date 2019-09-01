import websockets
import asyncio
import json
import logging
import collections
from http.cookies import SimpleCookie
import hashlib

logging.basicConfig()

id_lock = asyncio.Lock()
last_id = 0
async def new_id():
	global last_id
	async with id_lock:
		_id = last_id
		last_id+=1
	return _id


class User:

	def __init__(self,ws):
		self.id = -1
		self.nick = None
		self.ws = ws
		self.pic = 'default.png'

	async def create(self):
		self.id = await new_id()
		self.nick = 'UNKNOWN_%d' % (self.id)

	async def update_img(self):
		session = session_from_headers(self.ws.request_headers)
		if not session:
			return
		m = hashlib.sha1()
		m.update(session.encode())
		self.pic = m.hexdigest()+'.png'

	def get(self):
		return {
			'id':self.id,
			'nick':self.nick,
			'pic':self.pic
		}

	async def send(self,payload):
		await self.ws.send(json.dumps(payload))

class ChatEntry:

	def __init__(self, msg, user):
		self.msg = msg
		self.user = user

	def get(self):
		return {
			'msg':self.msg,
			'user_id':self.user.id
		}

chat_log_lock = asyncio.Lock()
chat_log = collections.deque(maxlen=100)

CON_MAP = {}
SESSION_MAP={}

async def emit(data,exclude=None):
	if CON_MAP:
	    await asyncio.wait([c.send(json.dumps(data)) for c in filter(lambda x: x!=exclude,CON_MAP.keys())])

async def unregister(websocket):
	if not websocket in CON_MAP:
		return
	user = CON_MAP[websocket]
	del CON_MAP[websocket]
	payload={
		'method':'remove_participant',
		'data':user.get()
	}
	await emit(payload)

def session_from_headers(headers):
	if not 'Cookie' in headers:
		return None
	c = SimpleCookie()
	try:
		c.load(headers['Cookie'])
		if 'PHPSESSID' in c:
			session = c['PHPSESSID'].value
			return session
	except:
		return None
async def register(websocket):
	session = session_from_headers(websocket.request_headers)
	if session:
		if session in SESSION_MAP:
			user = SESSION_MAP[session]
			user.ws = websocket
		else:
			user = User(websocket)
			await user.create()
			SESSION_MAP[session]=user
	else:
		user = User(websocket)
		await user.create()

	payload = {
		'method':'your_user',
		'data':user.get()
	}
	await user.send(payload)

	for con,u in CON_MAP.items():
		payload = {
			'method':'new_participant',
			'data':u.get()
		}
		await user.send(payload)
	for e in chat_log:
		payload = {
			'method':'new_message',
			'data':e.get()
		}
		await user.send(payload)
	
	payload = {
		'method':'new_participant',
		'data':user.get()
	}

	await emit(payload)
	CON_MAP[websocket]=user

async def update_profile(user,data):
	if type(data) != str:
		return
	user.nick = data
	payload = {
		'method':'your_user',
		'data':user.get()
	}
	await user.send(payload)
	payload = {
		'method':'update_participant',
		'data':user.get()
	}
	await emit(payload,user)

async def update_img(user):
	await user.update_img()
	payload = {
		'method':'your_user',
		'data':user.get()
	}
	await user.send(payload)

	payload={
		'method':'update_participant_img',
		'data':user.get()
	}
	await emit(payload,user)

async def new_message(user,data):
	if type(data) != str:
		return
	entry = ChatEntry(data,user)
	async with chat_log_lock:
		chat_log.append(entry)
	payload = {
		'method':'new_message',
		'data':entry.get()
	}
	await emit(payload)

async def handle_message(msg,websocket):
	user =CON_MAP[websocket]
	if not user:
		return
	msg = json.loads(msg)
	if not ('method' in msg and 'data' in msg):
		return
	method = msg['method']
	data = msg['data']
	if not method:
		return
	
	if method== 'new_message':
		await new_message(user,data)
	elif method == 'update_profile':
		await update_profile(user,data)
	elif method == 'update_img':
		await update_img(user)
		



async def on_conn(websocket, path):
    await register(websocket)
    try:
        async for message in websocket:
            await handle_message(message,websocket)
    finally:
    	await unregister(websocket)


async def main():
	await websockets.serve(on_conn, port=1337)
	while True:
		await asyncio.sleep(1)


if __name__ == '__main__':
	loop = asyncio.get_event_loop()
	try:
		loop.run_until_complete(main())
		#loop.run_forever()
	finally:
		loop.close()