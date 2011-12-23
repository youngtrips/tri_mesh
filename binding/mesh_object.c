#include <Python.h>

#include "structmember.h"

#include "mesh.h"

typedef struct {
    PyObject_HEAD
    mesh_t *mesh;
} MeshObject;

typedef struct {
    PyObject_HEAD
    PyObject *first; /* first name */
    PyObject *last;  /* last name */
    int number;
} Noddy;

static void
MeshObject_dealloc(MeshObject* self)
{
    printf("destroy mesh...\n");
    mesh_destroy(self->mesh);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *MeshObject_new(PyTypeObject *type,
        PyObject *args, PyObject *kwds)
{
    MeshObject *self;

    self = (MeshObject*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->mesh = mesh_init(NULL, 0);
    }
    return (PyObject *)self;
}

static int MeshObject_init(MeshObject *self, PyObject *args, PyObject *kwds)
{
    self->mesh = mesh_init(NULL, 0);
    printf("mesh init...\n");
    return 0;
}

static PyMemberDef MeshObject_members[] = {
    {"mesh", T_OBJECT_EX, offsetof(MeshObject, mesh), 0,
     "mesh handle"},
    {NULL}  /* Sentinel */
};

static PyObject *MeshObject_name(MeshObject* self)
{
    static PyObject *format = NULL;
    PyObject *args, *result;

    if (format == NULL) {
        format = PyString_FromString("%p");
        if (format == NULL)
            return NULL;
    }

    args = Py_BuildValue("OO", self);
    if (args == NULL)
        return NULL;

    result = PyString_Format(format, args);
    Py_DECREF(args);
    
    return result;
}

static PyObject *MeshObject_AddVertex(MeshObject *self, PyObject *args)
{
    printf("add vertex...\n");
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef MeshObject_methods[] = {
    {
        "name",
        (PyCFunction)MeshObject_name,
        METH_NOARGS,
        "Return the name"
    },
    {
        "add_vertex",
        (PyCFunction)MeshObject_AddVertex,
        METH_VARARGS,
        "Insert a vertex into mesh to generate new triangle mesh"
    },
   {NULL}  /* Sentinel */
};

static PyTypeObject MeshObjectType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "mesh.MeshObject",             /*tp_name*/
    sizeof(MeshObject),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)MeshObject_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Mesh objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    MeshObject_methods,             /* tp_methods */
    MeshObject_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)MeshObject_init,      /* tp_init */
    0,                         /* tp_alloc */
    MeshObject_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

void initmesh() 
{
    PyObject* m;

    if (PyType_Ready(&MeshObjectType) < 0)
        return;

    m = Py_InitModule3("mesh", module_methods,
                       "mesh module that creates an Mesh Object.");

    if (m == NULL)
      return;

    Py_INCREF(&MeshObjectType);
    PyModule_AddObject(m, "MeshObject", (PyObject *)&MeshObjectType);
}


