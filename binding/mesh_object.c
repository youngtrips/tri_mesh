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
    PyObject *list;
    PyObject *tuple;
    PyObject *x_obj;
    PyObject *y_obj;
    Py_ssize_t i;
    vertex_t *verties;
    double x;
    double y;
    int num;
    int ret;

    ret = PyArg_ParseTuple(args, "O!i", &PyList_Type, &list, &num);
    if (num == 0) {
        self->mesh = mesh_init(NULL, 0);
    } else {
        verties = (vertex_t*)calloc(num, sizeof(vertex_t));
        for (i = 0;i < num; i++) {
            tuple = PyList_GetItem(list, i);
            x_obj = PyTuple_GetItem(tuple, 0);
            y_obj = PyTuple_GetItem(tuple, 1);
            x = PyFloat_AsDouble(x_obj);
            y = PyFloat_AsDouble(y_obj);
            INIT_VERTEX(&verties[i], x, y);
        }
        self->mesh = mesh_init(verties, num);
    }
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
    double x;
    double y;
    Py_INCREF(Py_None);

    PyArg_ParseTuple(args, "dd", &x, &y);
    mesh_add_vertex(self->mesh, x, y);
    return Py_None;
}

static PyObject *MeshObject_DumpVertex(MeshObject *self, PyObject *args)
{
    Py_INCREF(Py_None);
    mesh_dump_vertex(self->mesh);
    return Py_None;
}

static PyObject *MeshObject_DumpEdge(MeshObject *self, PyObject *args)
{
    Py_INCREF(Py_None);
    mesh_dump_edge(self->mesh);
    return Py_None;
}

static PyObject *MeshObject_DumpTriangle(MeshObject *self, PyObject *args)
{
    Py_INCREF(Py_None);
    mesh_dump_triangle(self->mesh);
    return Py_None;
}

static PyObject *MeshObject_GetTriangles(MeshObject *self, PyObject *args)
{
    PyObject *tri_list;
    PyObject *tri_obj;
    PyObject *vert_obj;
    dlist_t *i;
    half_edge_t *he;
    triangle_t *tri;
    int p;

    if (self->mesh == NULL) {
        printf("mesh handle is NULL...\n");
        return NULL;
    }
    tri_list = PyList_New(0);
    if (tri_list == NULL) {
        printf("Create PyList Failed...\n");
        return NULL;
    }
    dlist_foreach(i, &(self->mesh->tlist)) {
        tri = (triangle_t*)dlist_entry(i, triangle_t, node);
        he = tri->edge;
        tri_obj = PyTuple_New(3);
        if (tri_obj == NULL) {
            printf("Create Tuple Failed...\n");
            Py_DECREF(tri_list);
            return NULL;
        }
        p = 0;
        do {
            if (he) {
                vert_obj = Py_BuildValue("(dd)", he->vert->x, he->vert->y);
                PyTuple_SetItem(tri_obj, p, vert_obj);
                p++;
                he = he->next;
            }
        } while(he && he != tri->edge);
        PyList_Append(tri_list, tri_obj);
    }
    return tri_list;
}

//int mesh_set_box(mesh_t *self, double left, double top, int width, int height);
static PyObject *MeshObject_SetBox(MeshObject *self, PyObject *args)
{
    double left;
    double top;
    double w;
    double h;

    Py_INCREF(Py_None);
    if (PyArg_ParseTuple(args, "dddd", &left, &top, &w, &h) < 0) {
        printf("Parse Args Failed...\n");
        return Py_None;
    }
    if (self->mesh == NULL) {
        printf("mesh handler is NULL...\n");
        return Py_None;
    }
    mesh_set_box(self->mesh, left, top, w, h);
    return Py_None;
}

static PyMethodDef MeshObject_methods[] = {
    {
        "add_vertex",
        (PyCFunction)MeshObject_AddVertex,
        METH_VARARGS,
        "Insert a vertex into mesh to generate new triangle mesh"
    },
    {
        "dump_vertex",
        (PyCFunction)MeshObject_DumpVertex,
        METH_NOARGS,
        "dump verties..."
    },
    {
        "dump_edge",
        (PyCFunction)MeshObject_DumpEdge,
        METH_NOARGS,
        "dump half edges..."
    },
    {
        "dump_triangle",
        (PyCFunction)MeshObject_DumpTriangle,
        METH_NOARGS,
        "dump triangles..."
    },
    {
        "triangles",
        (PyCFunction)MeshObject_GetTriangles,
        METH_NOARGS,
        "Get triangles..."
    },
    {
        "set_box",
        (PyCFunction)MeshObject_SetBox,
        METH_VARARGS,
        "Set Bounding box..."
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


