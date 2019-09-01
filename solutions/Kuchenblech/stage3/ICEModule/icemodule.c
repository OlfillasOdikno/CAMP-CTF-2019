/*
 * icemodule.c
 *
 *  Created on: 26.07.2019
 *      Author: localo
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ice.h"
#include <stdlib.h>
#include <stdio.h>

static PyObject *
ice_decrypt(PyObject *self, PyObject *args)
{
	const unsigned char* key ;
	Py_ssize_t key_count;
	const unsigned char* data ;
	Py_ssize_t data_count;
    PyObject * result;

	unsigned char* ret;
	int bytes_left;
	int block_size;


	if (!PyArg_ParseTuple(args, "s#s#", &data,&data_count ,&key,&key_count))
		return NULL;


	ICE_KEY* ik = ice_key_create(2);


	ice_key_set(ik, key);

	block_size = ice_key_block_size(ik);

	ret = malloc(data_count);

	bytes_left = data_count;
	while(bytes_left >= block_size){
		ice_key_decrypt(ik, data, ret);
		data+=block_size;
		ret+=block_size;
		bytes_left-=block_size;
	}
	for(int i = 0; i< bytes_left;i++){
		ret[i]=data[i];
	}

	ret-=(data_count-bytes_left);

	result = PyByteArray_FromStringAndSize((char*)ret, data_count);
	free(ret);

	ice_key_destroy(ik);

	return result;
}

static PyObject *
ice_encrypt(PyObject *self, PyObject *args)
{
	const unsigned char* key ;
	Py_ssize_t key_count;
	const unsigned char* data ;
	Py_ssize_t data_count;
    PyObject * result;

	unsigned char* ret;
	int bytes_left;
	int block_size;


	if (!PyArg_ParseTuple(args, "s#s#", &data,&data_count ,&key,&key_count))
		return NULL;


	ICE_KEY* ik = ice_key_create(2);


	ice_key_set(ik, key);

	block_size = ice_key_block_size(ik);

	ret = malloc(data_count);

	bytes_left = data_count;
	while(bytes_left >= block_size){
		ice_key_encrypt(ik, data, ret);
		data+=block_size;
		ret+=block_size;
		bytes_left-=block_size;
	}
	for(int i = 0; i< bytes_left;i++){
		ret[i]=data[i];
	}

	ret-=(data_count-bytes_left);

	result = PyByteArray_FromStringAndSize((char*)ret, data_count);
	free(ret);

	ice_key_destroy(ik);

	return result;
}



static PyMethodDef methods[] =
{
	{"decrypt", ice_decrypt, METH_VARARGS, "Decrypt ICE"},
	{"encrypt", ice_encrypt, METH_VARARGS, "Encrypt ICE"},
    { NULL, NULL, 0, NULL}
};

static struct PyModuleDef icemodule =
{
    PyModuleDef_HEAD_INIT,
    "ice",
    NULL,
    -1,
	methods
};

PyMODINIT_FUNC
PyInit_ice(void)
{
    return PyModule_Create(&icemodule);
}


