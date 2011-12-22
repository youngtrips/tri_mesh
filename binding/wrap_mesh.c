/*
 * Author: youngtrips
 * Created Time:  Fri 23 Dec 2011 12:28:21 AM CST
 * File Name: wrap_mesh.c
 * Description: 
 */

#include <Python.h>

#include "mesh.h"

PyObject *wrap_mesh_init(PyObject *self, PyObject *args)
{
    PyObject *vertex_list;
    PyObject *tuple_obj;
    PyObject *item_obj;
    Py_ssize_t i;
    mesh_t *mesh;
    vertex_t *verties;
    double x;
    double y;
    int num;
    int ret;

    ret = PyArg_ParseTuple(args, "O!i", &PyList_Type, &vertex_list, &num);
    if (vertex_list == NULL) {
        printf("Get list failed...\n");
    }
    ret = PyList_Check(vertex_list);
    printf("ret=%d\n", ret);
    if (num == 0) {
        mesh = mesh_init(NULL, 0);
    } else {
        verties = (vertex_t*)calloc(num, sizeof(verties));
        for (i = 0;i < num; i++) {
            tuple_obj = PyList_GetItem(vertex_list, i);
            item_obj = PyTuple_GetItem(tuple_obj, 0);
            x = PyFloat_AsDouble(item_obj);
            item_obj = PyTuple_GetItem(tuple_obj, 1);
            y = PyFloat_AsDouble(item_obj);
            printf("(%lf, %lf)\n", x, y);
            INIT_VERTEX(&(verties[i]), x, y);
        }
        printf("num = %d\n", num);
        mesh = mesh_init(verties, num);
        free(verties);
    }
    return Py_BuildValue("O", mesh);
}

static PyMethodDef mesh_methods[] = {
    {"init", wrap_mesh_init, METH_VARARGS, "create mesh object"},
    {NULL, NULL}
};

void initmesh()
{
    PyObject *m;

    m = Py_InitModule("mesh", mesh_methods);
}



