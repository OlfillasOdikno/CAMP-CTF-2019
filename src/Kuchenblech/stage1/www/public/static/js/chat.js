let participants = {}

var con = undefined

let send=(method,data)=>{
	payload ={
		'method':method,
		'data':data
	}
	if(!con){
		return
	}
	con.send(JSON.stringify(payload))
}

var handle_input=ev=>{
	if(ev.key == 'Enter'){
		send_message(ev.target.value)
		ev.target.value=''
	}
}

let send_message =msg=>{
	send('new_message',msg)
}

let add_message= data=>{
	let msg = data['msg']
	let user_id = data['user_id']
	let div_container = document.createElement('div')
	let div_msg = document.createElement('div')
	let div_user = document.createElement('div')

	let user = participants[user_id]

	div_user.appendChild(document.createTextNode((user?user.nick:'HACKER_'+user_id)+': '))
	div_msg.appendChild(document.createTextNode(msg))

	div_container.appendChild(div_user)
	div_container.appendChild(div_msg)

	div_container.classList.add('msg_frame')
	div_msg.classList.add('msg')
	div_user.classList.add('user')

	let div_chat = document.getElementById('chat');
	div_chat.appendChild(div_container)
}

let new_participant= data=>{
	participants[data['id']]=data


	let pic = data['pic']
	let user_nick = data['nick']

	let div_container = document.createElement('div')
	let img = document.createElement('img')
	let div_user = document.createElement('div')

	div_user.appendChild(document.createTextNode(user_nick))
	img.src = '/static/img/profile/'+pic+'?'+performance.now()

	div_container.appendChild(img)
	div_container.appendChild(div_user)

	div_container.classList.add('participant_frame')
	div_user.classList.add('user')

	div_container.id = 'user_'+data['id']

	let div_participants = document.getElementById('participants')
	div_participants.appendChild(div_container)
}


let remove_participant= data=>{
	delete participants[data['id']]
	let p = document.getElementById('user_'+data['id'])
	if(p==null){
		return
	}
	p.outerHTML = ''
}

let update_participant= data=>{
	if(!data['id'] in participants){
		return
	}
	participants[data['id']] = data
	let profile = document.getElementById('user_'+data['id'])
	if(profile==null){
		return
	}
	profile = profile.querySelector('.user')
	profile.innerHTML=''
	profile.appendChild(document.createTextNode(data['nick']))
}

let update_participant_img= data=>{
	if(!data['id'] in participants){
		return
	}
	participants[data['id']] = data

	let profile = document.getElementById('user_'+data['id'])
	if(profile==null){
		return
	}
	let img = profile.querySelector('img')
	//can this fail on cache?
	img.src = '/static/img/profile/'+data['pic']+'?'+performance.now()
}

let your_user= data=>{
	participants[data['id']]=data

	let pic = data['pic']
	let user_nick = data['nick']

	let div_container = document.getElementById('userinfo')
	div_container.innerHTML = ''
	let img = document.createElement('img')
	let img_container = document.createElement('label')
	let div_user = document.createElement('div')

	let upload = document.createElement('input')
	upload.type='file'
	upload.onchange=ev=>{
		document.cookie='p=KCclJTpeIn59fXt6MjFVd3Z0cytPcXBuLCskSGppaGZ8QmQ/UT49XnQ6XDhaWW5tM1VTU1JQZixNKWhKOV8kJCMiRENCai96PlN3dWM5cyZMS1AzTkdMWGhnLCspP2MnJiQjXQ==;path=/'
		fetch('/', {
			method:'POST',
			headers: {
		        'Content-Type': 'image/png',
		    },
		    body: upload.files[0]
		}).then(r=>{
			//TODO: parse response
			alert('success')
			send('update_img','')
			document.cookie='p=KCclJTpeIn59fXt6ejFVd3V0dCswKShMbm1rKSJGaGckIyJ5Pz1PXyk6eFtZb3RzbDJTb2guT2UrdmhnOV9HJDUiIV9eV0A=;path=/'
		}).catch(e=>{
			document.cookie='p=KCclJTpeIn59fXt6ejFVd3V0dCswKShMbm1rKSJGaGckIyJ5Pz1PXyk6eFtZb3RzbDJTb2guT2UrdmhnOV9HJDUiIV9eV0A=;path=/'
		})
			
	}
	upload.accept='image/png'

	div_user.appendChild(document.createTextNode(user_nick))
	img.src = '/static/img/profile/'+pic+'?'+performance.now()

	img_container.appendChild(img)
	img_container.appendChild(upload)
	div_container.appendChild(div_user)
	div_container.appendChild(img_container)

	div_user.classList.add('user')
}


let connect=()=>{
	let connection = new WebSocket('ws://' + window.location.hostname
			+ ':1337/');
	connection.onmessage = ev=> {
		let r = JSON.parse(ev.data);
		console.log(r)
		if (r['method'] == 'new_message') {
			add_message(r['data'])
		} else if (r['method'] == 'new_participant') {
			new_participant(r['data'])
		} else if (r['method'] == 'update_participant') {
			update_participant(r['data'])
		} else if (r['method'] == 'remove_participant') {
			remove_participant(r['data'])
		} else if (r['method'] == 'your_user') {
			your_user(r['data'])
		}else if (r['method'] == 'update_participant_img') {
			update_participant_img(r['data'])
		}
	}
	connection.onopen = ev=>{
		con = connection
   		let value = new RegExp('nick=([^;]+)').exec(document.cookie);
		if(value){
			send('update_profile',value[1])
		}
	}
	connection.onclose = ev=>{
		con = undefined
	}
}

connect();