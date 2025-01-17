/* GTS - Library for the manipulation of triangulated surfaces
 * Copyright (C) 1999 Stéphane Popinet
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTS_H__
#define __GTS_H__

#include <stdio.h>
#include <glib.h>
#include "cexcept.h"
#ifndef NATIVE_WIN32
# include <gtsconfig.h>
#endif
#ifdef GTS_COMPILATION
# include "config.h"
#endif /* not GTS_COMPILATION */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Added based on glib.h by M J Loehr 01/01/01 */
/* GTS version.
 * we prefix variable declarations so they can
 * properly get exported in windows dlls.
 */
#ifdef NATIVE_WIN32
#  ifdef GTS_COMPILATION
#    define GTS_C_VAR __declspec(dllexport)
#  else /* not GTS_COMPILATION */
#    define GTS_C_VAR extern __declspec(dllimport)
#  endif /* not GTS_COMPILATION */
#else /* not NATIVE_WIN32 */
#  define GTS_C_VAR extern
#endif /* not NATIVE_WIN32 */

GTS_C_VAR const guint gts_major_version;
GTS_C_VAR const guint gts_minor_version;
GTS_C_VAR const guint gts_micro_version;
GTS_C_VAR const guint gts_interface_age;
GTS_C_VAR const guint gts_binary_age;

#define GTS_CHECK_VERSION(major,minor,micro)    \
    (gts_major_version > (major) || \
    (gts_major_version == (major) && gts_minor_version > (minor)) || \
    (gts_major_version == (major) && gts_minor_version == (minor) && \
     gts_micro_version >= (micro)))

#define GTS_DELIMITERS " \t"
#define GTS_COMMENTS   "#!"
#define GTS_TOKENS     "\n{}()="

#define GTS_OBJ_DELIMITERS " \t\n\\"
#define GTS_OBJ_COMMENTS   "#g"
#define GTS_OBJ_TOKENS     ""

#define GTS_MAINTAINER "luis94855510@gmail.com"
#define USE_SURFACE_BTREE 1

/* Class declarations for base types */

typedef struct _GtsObjectClassInfo     GtsObjectClassInfo;
typedef struct _GtsObject        GtsObject;
typedef struct _GtsObjectClass   GtsObjectClass;
typedef struct _GtsException       GtsException;
typedef struct _GtsExceptionClass  GtsExceptionClass;
typedef struct _GtsPoint         GtsPoint;
typedef struct _GtsPointClass    GtsPointClass;
typedef struct _GtsVertex        GtsVertex;
typedef struct _GtsVertexClass   GtsVertexClass;
typedef struct _GtsSegment       GtsSegment;
typedef struct _GtsSegmentClass  GtsSegmentClass;
typedef struct _GtsEdge          GtsEdge;
typedef struct _GtsEdgeClass     GtsEdgeClass;
typedef struct _GtsTriangle      GtsTriangle;
typedef struct _GtsTriangleClass GtsTriangleClass;
typedef struct _GtsFace          GtsFace;
typedef struct _GtsFaceClass     GtsFaceClass;
typedef struct _GtsBBox          GtsBBox;
typedef struct _GtsBBoxClass     GtsBBoxClass;
typedef struct _GtsSurface       GtsSurface;
typedef struct _GtsSurfaceClass  GtsSurfaceClass;

/**
 * GtsObjectClassInitFunc:
 * @objclass: a #GtsObjectClass to be initialized.
 *
 * A function to call to initialize an object class.
 */
typedef void (*GtsObjectClassInitFunc) (GtsObjectClass * objclass);

/**
 * GtsObjectInitFunc:
 * @obj: a #GtsObject.
 *
 * A function to call to initialize an object.
 */
typedef void (*GtsObjectInitFunc) (GtsObject * obj);

/**
 * GtsArgSetFunc:
 * @obj: a #GtsObject.
 *
 * Defined for future use.
 */
typedef void (*GtsArgSetFunc) (GtsObject * obj);

/**
 * GtsArgGetFunc:
 * @obj: a #GtsObject.
 *
 * Defined for future use.
 */
typedef void (*GtsArgGetFunc) (GtsObject * obj);

typedef gdouble                  GtsVector[3];
typedef gdouble                  GtsVector4[4];

/**
 * GtsMatrix:
 *
 * A #GtsMatrix is a 3x3 matrix.
 */
typedef GtsVector4               GtsMatrix;

/**
 * GtsKeyFunc:
 * @item: A pointer to an item to be stored in the heap.
 * @data: User data passed to gts_eheap_new().
 *
 * Returns: the value of the key for the given item.
 */
typedef gdouble                  (*GtsKeyFunc)    (gpointer item,
                                                   gpointer data);
/**
 * GtsIntersect:
 * @GTS_OUT: the intersection is empty.
 * @GTS_ON: the intersection is a full set.
 * @GTS_IN: the intersection is a thin set.
 *
 * Enum values used for the intersection set,
 * to specify the type of intersection.
 **/
typedef enum
{
  GTS_OUT = -1,
  GTS_ON = 0,
  GTS_IN = 1
} GtsIntersect;

typedef struct _GtsColor         GtsColor;

struct _GtsColor {
  gfloat r, g, b;
};

typedef gint   (*GtsFunc)              (gpointer item,
                                        gpointer data);

/* misc.c */

typedef struct _GtsFile GtsFile;

typedef enum {
  GTS_NONE   = 1 << 8,
  GTS_INT    = 1 << 9,
  GTS_UINT   = 1 << 10,
  GTS_FLOAT  = 1 << 11,
  GTS_DOUBLE = 1 << 12,
  GTS_STRING = 1 << 13,
  GTS_FILE   = 1 << 14,
  GTS_ERROR  = 1 << 15
} GtsTokenType;

typedef enum {
  GTS_FILE_GTS = 1 << 0,
  GTS_FILE_OBJ = 1 << 1,
} GtsFileType;

struct _GtsFile {
  FILE * fp;
  gchar * s, * s1;
  guint line, pos;
  GString * token;
  GtsTokenType type;
  gchar * error;

  guint curline, curpos;
  guint scope, scope_max;
  gint next_token;
  gchar * delimiters;
  gchar * comments;
  gchar * tokens;

  GtsFileType ftype;
};

typedef struct _GtsFileVariable GtsFileVariable;

struct _GtsFileVariable {
  GtsTokenType type;
  gchar name[30];
  gboolean unique;
  gpointer data;
  gboolean set;
  guint line, pos;
};


GtsFile *      gts_file_new                 (FILE * fp);
GtsFile *      gts_file_new_obj             (FILE * fp);
GtsFile *      gts_file_new_from_string     (const gchar * s);
GtsFile *      gts_file_new_from_string_obj (const gchar * s);
void           gts_file_verror              (GtsFile * f,
                                             const gchar * format,
                                             va_list args);
void           gts_file_error               (GtsFile * f,
                                             const gchar * format,
                                             ...);
gint           gts_file_getc                (GtsFile * f);
guint          gts_file_read                (GtsFile * f,
                                             gpointer ptr,
                                             guint size,
                                             guint nmemb);
gint           gts_file_getc_scope          (GtsFile * f);
void           gts_file_next_token          (GtsFile * f);
void           gts_file_first_token_after   (GtsFile * f,
                                             GtsTokenType type);
void           gts_file_set_type            (GtsFile * f,
                                             GtsFileType ftype);
void           gts_file_assign_start        (GtsFile * f,
                                             GtsFileVariable * vars);
GtsFileVariable * gts_file_assign_next      (GtsFile * f,
                                             GtsFileVariable * vars);
void           gts_file_assign_variables    (GtsFile * f,
                                             GtsFileVariable * vars);
void           gts_file_variable_error      (GtsFile * f,
                                             GtsFileVariable * vars,
                                             const gchar * name,
                                             const gchar * format,
                                             ...);
void           gts_file_destroy             (GtsFile * f);

/* Objects: object.c */

/**
 * GTS_OBJECT_CAST:
 * @obj: a #GtsObject.
 * @type: the object type to cast @obj to (example: GtsObject).
 * @klass: the object class to cast @obj to (example: gts_object()).
 *
 * This macro is used to define casting macros for object.
 * You should need it only when writing new object functions.
 */

/**
 * GTS_OBJECT_CLASS_CAST:
 * @objklass: a #GtsObjectClass.
 * @type: The class type to cast @objklass to (example: GtsObjectClass).
 * @klass: the class to cast @objklass to (example: gts_object_class()).
 *
 * This macro is used to define casting macros for object classes.
 * You should need it only when writing new object class functions.
 */
#ifdef GTS_CHECK_CASTS
#  define GTS_OBJECT_CAST(obj, type, klass) ((type *) gts_object_check_cast (obj, klass))
#  define GTS_OBJECT_CLASS_CAST(objklass, type, klass) ((type *) gts_object_class_check_cast (objklass, klass))
#else  /* not GTS_CHECK_CASTS */
#  define GTS_OBJECT_CAST(obj, type, klass)             ((type *) (obj))
#  define GTS_OBJECT_CLASS_CAST(objklass, type, klass)  ((type *) (objklass))
#endif /* not GTS_CHECK_CASTS */

/**
 * GTS_CLASS_NAME_LENGTH:
 *
 * Maximum class name length for #GtsObjectClass.
 */
#define GTS_CLASS_NAME_LENGTH 40

/**
 * GTS_OBJECT:
 * @obj: a descendant of #GtsObject.
 *
 * Casts @obj to #GtsObject.
 */
#define GTS_OBJECT(obj) GTS_OBJECT_CAST (obj,\
                                         GtsObject,\
                                         gts_object_class ())

/**
 * GTS_OBJECT_CLASS:
 * @klass: a descendant of #GtsObjectClass.
 *
 * Casts @klass to #GtsObjectClass.
 */
#define GTS_OBJECT_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                       GtsObjectClass,\
                                                       gts_object_class())
/**
 * GTS_IS_OBJECT:
 * @obj: a pointer to test.
 *
 * Evaluates to %TRUE if @obj is a #GtsObject, %FALSE otherwise.
 */
#define GTS_IS_OBJECT(obj) (gts_object_is_from_class (obj,\
                                                      gts_object_class ()))

typedef enum
{
  GTS_DESTROYED         = 1 << 0,
  GTS_USER_FLAG         = 1 /* user flags start from here */
} GtsObjectFlags;

#define GTS_OBJECT_FLAGS(obj)             (GTS_OBJECT (obj)->flags)
#define GTS_OBJECT_DESTROYED(obj)         ((GTS_OBJECT_FLAGS (obj) & GTS_DESTROYED) != 0)
#define GTS_OBJECT_SET_FLAGS(obj,flag)    G_STMT_START{ (GTS_OBJECT_FLAGS (obj) |= (flag)); }G_STMT_END
#define GTS_OBJECT_UNSET_FLAGS(obj,flag)  G_STMT_START{ (GTS_OBJECT_FLAGS (obj) &= ~(flag)); }G_STMT_END

/**
 * GtsObjectClassInfo:
 * @name: object class name.
 * @object_size: size of the object structure to be allocated
 * when constructing the object.
 * @class_size: size of the object class structure to be
 * allocated when instanciating the object class.
 * @object_init_func: object initialization function
 * @class_init_func: class initialization function.
 * @arg_set_func: defined for future use.
 * @arg_get_func: defined for future use.
 *
 * Informations about a #GtsObjectClass.
 */
struct _GtsObjectClassInfo {
  gchar name[GTS_CLASS_NAME_LENGTH];
  guint object_size;
  guint class_size;
  GtsObjectClassInitFunc class_init_func;
  GtsObjectInitFunc object_init_func;
  GtsArgSetFunc arg_set_func;
  GtsArgGetFunc arg_get_func;
};

/**
 * GtsObject:
 * @klass: The object class.
 * @reserved: Used internally by the library.
 * @flags: Used internally.
 *
 * The base object structure.
 */
struct _GtsObject {
  GtsObjectClass * klass;

  gpointer reserved;
  guint32 flags;
};

/**
 * GtsObjectClass:
 *
 * The base object class structure. All the virtual functions
 * @clone, @destroy, @read and @write can be overloaded. The
 * default @read and @write methods are undefined. The default
 * @clone method just copies the object structure. The default
 * @destroy method frees the memory allocated for a given object
 * structure.
 */
struct _GtsObjectClass {
  GtsObjectClassInfo info;
  GtsObjectClass * parent_class;

  void        (* clone)      (GtsObject *, GtsObject *);
  void        (* destroy)    (GtsObject *);
  void        (* read)       (GtsObject **, GtsFile *);
  void        (* write)      (GtsObject *, FILE *);
  GtsColor    (* color)      (GtsObject *);
  void        (* attributes) (GtsObject *, GtsObject *);
};

gpointer         gts_object_class_new      (GtsObjectClass * parent_class,
                                            GtsObjectClassInfo * info);
GtsObjectClass * gts_object_class          (void);
gpointer         gts_object_check_cast     (gpointer object,
                                            gpointer klass);
gpointer         gts_object_class_check_cast (gpointer klass,
                                              gpointer from);

static inline
gpointer gts_object_is_from_class (gpointer object,
                                   gpointer klass)
{
  GtsObjectClass * c;

  g_return_val_if_fail (klass != NULL, NULL);

  if (object == NULL)
    return NULL;

  c = ((GtsObject *) object)->klass;

  g_return_val_if_fail (c != NULL, NULL);

  while (c) {
    if (c == klass)
      return object;
    c = c->parent_class;
  }

  return NULL;
}

static inline
gpointer gts_object_class_is_from_class (gpointer klass,
                                         gpointer from)
{
  GtsObjectClass * c;

  g_return_val_if_fail (klass != NULL, NULL);
  g_return_val_if_fail (from != NULL, NULL);

  c = (GtsObjectClass *) klass;
  while (c) {
    if (c == from)
      return klass;
    c = c->parent_class;
  }

  return NULL;
}

GtsObjectClass * gts_object_class_from_name     (const gchar * name);

GtsObject *      gts_object_new                 (GtsObjectClass * klass);
GtsObject *      gts_object_clone               (GtsObject * object);
void             gts_object_attributes          (GtsObject * object,
                                                 GtsObject * from);
void             gts_object_init                (GtsObject * object,
                                                 GtsObjectClass * klass);
void             gts_object_reset_reserved      (GtsObject * object);
void             gts_object_destroy             (GtsObject * object);
void             gts_finalize                   (void);

/* Ranges: surface.c */
typedef struct _GtsRange               GtsRange;

struct _GtsRange {
  gdouble min, max, sum, sum2, mean, stddev;
  guint n;
};

void gts_range_init         (GtsRange * r);
void gts_range_reset        (GtsRange * r);
void gts_range_add_value    (GtsRange * r,
                             gdouble val);
void gts_range_update       (GtsRange * r);
void gts_range_print        (GtsRange * r,
                             FILE * fptr);

/* Exceptions: exception.c */

/**
 * GTS_IS_EXCEPTION:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsException, %FALSE otherwise.
 */
#define GTS_IS_EXCEPTION(obj) (gts_object_is_from_class (obj,\
                                                         gts_exception_class ()))

/**
 * GTS_EXCEPTION:
 * @obj: a descendant of #GtsException.
 *
 * Casts @obj to #GtsException.
 */
#define GTS_EXCEPTION(obj) GTS_OBJECT_CAST (obj,\
                                            GtsException,\
                                            gts_exception_class ())
/**
 * GTS_EXCEPTION_CLASS:
 * @klass: a #GtsExceptionClass.
 *
 * Casts @klass to #GtsExceptionClass.
 */
#define GTS_EXCEPTION_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                          GtsExceptionClass,\
                                                          gts_exception_class ())

/**
 * GtsExceptionError:
 * @GTS_ASSERTION_ERROR: Thrown when an assert statement fails.
 * @GTS_IO_ERROR: Thrown when an I/O operation fails.
 * @GTS_RUNTIME_ERROR: Thrown when an error is detected that
 * doesn’t fall in any of the other categories.
 * @GTS_VALUE_ERROR: Thrown when a built-in operation or function
 * receives an argument that has the right type but an inappropriate value.
 *
 * Enum values used for the built-in exceptions.
 */
typedef enum {
  GTS_ASSERTION_ERROR,
  GTS_IO_ERROR,
  GTS_RUNTIME_ERROR,
  GTS_VALUE_ERROR
} GtsExceptionError;

/**
 * GtsException:
 * @object: The parent object.
 * @error: The error type.
 * @message: The error message that explains
 * the reason for the exception.
 *
 * The exception object.
 */
struct _GtsException {
  GtsObject object;

  GtsExceptionError error;
  gchar * message;
};

/**
 * GtsExceptionClass:
 * @parent_class: The parent class.
 *
 * The exception class. No virtual functions are associated.
 */
struct _GtsExceptionClass {
  GtsObjectClass parent_class;
};

define_exception_type(GtsException *);
extern struct exception_context the_exception_context[1];

GtsExceptionClass * gts_exception_class              (void);
GtsException *      gts_exception_new_full           (GtsExceptionClass * klass,
                                                      GtsExceptionError error,
                                                      gchar const * format, ...);
gint                gts_exception_set_message        (GtsException * e, gchar const * format, ...);
/**
 * gts_exception_new:
 * @error: a #GtsExceptionError.
 * @format: a standard printf() format string for the
 * explanation of the error.
 * @...: the arguments to insert in the explanation of
 * the error.
 *
 * Returns: a new #GtsException.
 */
#define             gts_exception_new(error, format, ...) \
                      gts_exception_new_full (gts_exception_class (), \
                                              error, \
                                              "[%s:%d] " format, \
                                              __FILE__, __LINE__, \
                                              ##__VA_ARGS__)
/**
 * gts_exception_fprintf:
 * @file: the stream to write to.
 * @exception: a #GtsException.
 * @format: a standard printf() format string.
 * @...: the arguments to insert in the output.
 *
 * Returns: the number of bytes printed.
 */
#define             gts_exception_fprintf(file, exception, format, ...) \
                      g_fprintf (file, "%s  \n[%s:%d] " format, \
                                        exception->message, __FILE__, __LINE__, \
                                        ##__VA_ARGS__)
/**
 * gts_exception_printf:
 * @exception: a #GtsException.
 * @format: a standard printf() format string.
 * @...: the arguments to insert in the output.
 *
 * Returns: the number of bytes printed.
 */
#define             gts_exception_printf(exception, format, ...) \
                      gts_exception_fprintf (stderr, exception, \
                                             format, ##__VA_ARGS__)
/**
 * gts_exception_append:
 * @exception: a #GtsException.
 * @format: a standard printf() format string.
 * @...: the arguments to append in the explanation
 * of @exception (i.e. @exception->message).
 *
 * Returns: the new number of bytes in the explanation.
 */
#define             gts_exception_append(exception, format, ...) \
                      gts_exception_set_message (exception, "%s  \n[%s:%d] " format, \
                      exception->message, __FILE__, __LINE__, ##__VA_ARGS__)

/* Points: point.c */

/**
 * GTS_IS_POINT:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a descendant of a #GtsPoint, %FALSE otherwise.
 */
#define GTS_IS_POINT(obj) (gts_object_is_from_class (obj,\
                                                     gts_point_class ()))
/**
 * GTS_POINT:
 * @obj: a descendant of #GtsPoint.
 *
 * Casts @obj to #GtsPoint.
 */
#define GTS_POINT(obj) GTS_OBJECT_CAST (obj,\
                                        GtsPoint,\
                                        gts_point_class ())
/**
 * GTS_POINT_CLASS:
 * @klass: a descendant of #GtsPointClass.
 *
 * Casts @klass to #GtsPointClass.
 */
#define GTS_POINT_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                      GtsPointClass,\
                                                      gts_point_class ())

/**
 * GtsPoint:
 * @object: The parent object.
 * @x: x coordinate.
 * @y: y coordinate.
 * @z: z coordinate.
 *
 * The point object.
 */
struct _GtsPoint {
  GtsObject object;

  gdouble x, y, z; /* must be contiguous (cast to robust functions) */
};

/**
 * GtsPointClass:
 * @parent_class: The parent class.
 * @binary: %True if the I/O operation is done in binary.
 *
 * The point class. No virtual functions are associated.
 */
struct _GtsPointClass {
  GtsObjectClass parent_class;
  gboolean binary;
};

GtsPointClass * gts_point_class                      (void);
GtsPoint *    gts_point_new                          (GtsPointClass * klass,
                                                      gdouble x,
                                                      gdouble y,
                                                      gdouble z);
void          gts_point_set                          (GtsPoint * p,
                                                      gdouble x,
                                                      gdouble y,
                                                      gdouble z);
/**
 * gts_point_is_in_rectangle:
 * @p: a #GtsPoint.
 * @p1: the lower-left front corner of the box.
 * @p2: the upper-right back corner of the box.
 *
 * Evaluates to %TRUE if @p is contained in the box (boundary included)
 * defined by its two corners @p1 and @p2, %FALSE otherwise.
 */
#define       gts_point_is_in_rectangle(p, p1, p2)   ((p)->x >= (p1)->x &&\
                                                      (p)->x <= (p2)->x &&\
                                                      (p)->y >= (p1)->y &&\
                                                      (p)->y <= (p2)->y &&\
                                                      (p)->z >= (p1)->z &&\
                                                      (p)->z <= (p2)->z)
GtsPoint *    gts_segment_triangle_intersection      (GtsSegment * s,
                                                      GtsTriangle * t,
                                                      gboolean boundary,
                                                      GtsPointClass * klass);
void          gts_point_transform                    (GtsPoint * p,
                                                      GtsMatrix * m);
gdouble       gts_point_dot                          (GtsPoint * p1,
                                                      GtsPoint * p2);
GtsPoint *    gts_point_cross                        (GtsPoint * p1,
                                                      GtsPoint * p2);
GtsPoint *    gts_point_normal                       (GtsPoint * p1,
                                                      GtsPoint * p2,
                                                      GtsPoint * p3);
gdouble       gts_point_distance                     (GtsPoint * p1,
                                                      GtsPoint * p2);
gdouble       gts_point_distance2                    (GtsPoint * p1,
                                                      GtsPoint * p2);
gdouble       gts_point_orientation_3d               (GtsPoint * p1,
                                                      GtsPoint * p2,
                                                      GtsPoint * p3,
                                                      GtsPoint * p4);
gint          gts_point_orientation_3d_sos           (GtsPoint * p1,
                                                      GtsPoint * p2,
                                                      GtsPoint * p3,
                                                      GtsPoint * p4);
GtsIntersect  gts_point_is_in_triangle               (GtsPoint * p,
                                                      GtsTriangle * t);
gdouble       gts_point_in_circle                    (GtsPoint * p,
                                                      GtsPoint * p1,
                                                      GtsPoint * p2,
                                                      GtsPoint * p3);
gdouble       gts_point_in_sphere                    (GtsPoint * p,
                                                      GtsPoint * p1,
                                                      GtsPoint * p2,
                                                      GtsPoint * p3,
                                                      GtsPoint * p4);
gdouble       gts_point_in_triangle_circle           (GtsPoint * p,
                                                      GtsTriangle * t);
gdouble       gts_point_orientation                  (GtsPoint * p1,
                                                      GtsPoint * p2,
                                                      GtsPoint * p3);
gint          gts_point_orientation_sos              (GtsPoint * p1,
                                                      GtsPoint * p2,
                                                      GtsPoint * p3);
gdouble       gts_point_segment_distance2            (GtsPoint * p,
                                                      GtsSegment * s);
gdouble       gts_point_segment_distance             (GtsPoint * p,
                                                      GtsSegment * s);
void          gts_point_segment_closest              (GtsPoint * p,
                                                      GtsSegment * s,
                                                      GtsPoint * closest);
gdouble       gts_point_triangle_distance2           (GtsPoint * p,
                                                      GtsTriangle * t);
gdouble       gts_point_triangle_distance            (GtsPoint * p,
                                                      GtsTriangle * t);
void          gts_point_triangle_closest             (GtsPoint * p,
                                                      GtsTriangle * t,
                                                      GtsPoint * closest);
gboolean      gts_point_is_inside_surface            (GtsPoint * p,
                                                      GNode * tree,
                                                      gboolean is_open);

/* Vertices: vertex.c */

/**
 * GTS_IS_VERTEX:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsVertex, %FALSE otherwise.
 */
#define GTS_IS_VERTEX(obj) (gts_object_is_from_class (obj,\
                                                      gts_vertex_class ()))
/**
 * GTS_VERTEX:
 * @obj: a descendant of #GtsVertex.
 *
 * Casts @obj to #GtsVertex.
 */
#define GTS_VERTEX(obj) GTS_OBJECT_CAST (obj,\
                                         GtsVertex,\
                                         gts_vertex_class ())
/**
 * GTS_VERTEX_CLASS:
 * @klass: a descendant of #GtsVertexClass.
 *
 * Casts @klass to #GtsVertexClass.
 */
#define GTS_VERTEX_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                       GtsVertexClass,\
                                                       gts_vertex_class ())

/**
 * GtsVertex:
 * @p: The parent object.
 * @segments: Contains all the #GtsSegment using this vertex as one of their endpoints.
 *
 * The vertex object.
 */
struct _GtsVertex {
  GtsPoint p;

  GSList * segments;
};

/**
 * GtsVertexClass:
 * @parent_class: The parent class.
 *
 * The vertex class. No virtual functions are associated.
 */
struct _GtsVertexClass {
  GtsPointClass parent_class;

  /*< private >*/
  void        (* intersection_attributes) (GtsVertex *,
                                           GtsObject *,
                                           GtsObject *);
};

GTS_C_VAR
gboolean      gts_allow_floating_vertices;

GtsVertexClass * gts_vertex_class          (void);
GtsVertex *   gts_vertex_new               (GtsVertexClass * klass,
                                            gdouble x,
                                            gdouble y,
                                            gdouble z);
void          gts_vertex_replace           (GtsVertex * v,
                                            GtsVertex * with);
gboolean      gts_vertex_is_unattached     (GtsVertex * v);
GtsSegment *  gts_vertices_are_connected   (GtsVertex * v1,
                                            GtsVertex * v2);
GSList *      gts_vertex_triangles         (GtsVertex * v,
                                            GSList * list);
GSList *      gts_vertex_faces             (GtsVertex * v,
                                            GtsSurface * surface,
                                            GSList * list);
GSList *      gts_vertex_neighbors         (GtsVertex * v,
                                            GSList * list,
                                            GtsSurface * surface);
GSList *      gts_vertices_from_segments   (GSList * segments);
gboolean      gts_vertex_is_boundary       (GtsVertex * v,
                                            GtsSurface * surface);
GList *       gts_vertices_merge           (GList * vertices,
                                            gdouble epsilon,
                                            gboolean (* check) (GtsVertex *, GtsVertex *));
GSList *      gts_vertex_fan_oriented      (GtsVertex * v,
                                            GtsSurface * surface);
guint         gts_vertex_is_contact        (GtsVertex * v, gboolean sever);

/* GtsVertexNormal: Header */

typedef struct _GtsVertexNormal         GtsVertexNormal;

struct _GtsVertexNormal {
  /*< private >*/
  GtsVertex parent;

  /*< public >*/
  GtsVector n;
};

#define GTS_VERTEX_NORMAL(obj)            GTS_OBJECT_CAST (obj,\
                                                 GtsVertexNormal,\
                                                 gts_vertex_normal_class ())
#define GTS_IS_VERTEX_NORMAL(obj)         (gts_object_is_from_class (obj,\
                                                 gts_vertex_normal_class ()))

GtsVertexClass * gts_vertex_normal_class  (void);

/* GtsColorVertex: Header */

typedef struct _GtsColorVertex         GtsColorVertex;

struct _GtsColorVertex {
  /*< private >*/
  GtsVertex parent;

  /*< public >*/
  GtsColor c;
};

#define GTS_COLOR_VERTEX(obj)            GTS_OBJECT_CAST (obj,\
                                                 GtsColorVertex,\
                                                 gts_color_vertex_class ())
#define GTS_IS_COLOR_VERTEX(obj)         (gts_object_is_from_class (obj,\
                                                 gts_color_vertex_class ()))

GtsVertexClass * gts_color_vertex_class  (void);

/* Segments: segment.c */

/**
 * GTS_IS_SEGMENT:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a descendant of #GtsSegment, %FALSE otherwise.
 */
#define GTS_IS_SEGMENT(obj) (gts_object_is_from_class (obj,\
                                                       gts_segment_class ()))

/**
 * GTS_SEGMENT:
 * @obj: a descendant of #GtsSegment.
 *
 * Casts @obj to #GtsSegment.
 */
#define GTS_SEGMENT(obj) GTS_OBJECT_CAST (obj,\
                                          GtsSegment,\
                                          gts_segment_class ())

/**
 * GTS_SEGMENT_CLASS:
 * @klass: a descendant of #GtsSegmentClass.
 *
 * Casts @klass to #GtsSegmentClass.
 */
#define GTS_SEGMENT_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                        GtsSegmentClass,\
                                                        gts_segment_class ())

/**
 * GtsSegment:
 * @object: The parent object.
 * @v1: First endpoint of the segment.
 * @v2: Second endpoint of the segment.
 *
 * The segment object.
 */
struct _GtsSegment {
  GtsObject object;

  GtsVertex * v1;
  GtsVertex * v2;
};

/**
 * GtsSegmentClass:
 * @parent_class: The parent class.
 *
 * The segment class. No virtual functions are associated.
 */
struct _GtsSegmentClass {
  GtsObjectClass parent_class;
};

/**
 * gts_segment_connect:
 * @s: a #GtsSegment.
 * @u1: a #GtsVertex.
 * @u2: another #GtsVertex.
 *
 * Evaluates to %TRUE if @s connects @u1 with @u2, %FALSE otherwise.
 */
#define gts_segment_connect(s, u1, u2) (((s)->v1 == (u1) &&\
                                         (s)->v2 == u2)\
                                        ||\
                                        ((s)->v1 == u2 &&\
                                         (s)->v2 == u1))

/**
 * gts_segments_are_identical:
 * @s1: a #GtsSegment.
 * @s2: another #GtsSegment.
 *
 * Evaluates to %TRUE if @s1 and @s2 link the same vertices, %FALSE otherwise.
 */
#define gts_segments_are_identical(s1, s2) (((s1)->v1 == (s2)->v1 &&\
                                             (s1)->v2 == (s2)->v2)\
                                            ||\
                                            ((s1)->v1 == (s2)->v2 &&\
                                             (s1)->v2 == (s2)->v1))

/**
 * gts_segments_touch:
 * @s1: a #GtsSegment.
 * @s2: another #GtsSegment.
 *
 * Evaluates to %TRUE if @s1 and @s2 have a common vertex, %FALSE otherwise.
 */
#define gts_segments_touch(s1, s2) ((s1)->v1 == (s2)->v1 ||\
                                    (s1)->v1 == (s2)->v2 ||\
                                    (s1)->v2 == (s2)->v1 ||\
                                    (s1)->v2 == (s2)->v2)

GtsSegmentClass * gts_segment_class                  (void);
GtsSegment *      gts_segment_new                    (GtsSegmentClass * klass,
                                                      GtsVertex * v1,
                                                      GtsVertex * v2);
gdouble       gts_segment_length                     (GtsSegment * s);
gdouble       gts_segment_length2                    (GtsSegment * s);
GtsIntersect  gts_segments_are_intersecting          (GtsSegment * s1,
                                                      GtsSegment * s2);
GtsSegment *  gts_segment_is_duplicate               (GtsSegment * s);
GtsVertex *   gts_segment_midvertex                  (GtsSegment * s,
                                                      GtsVertexClass * klass);
GSList *      gts_segments_from_vertices             (GSList * vertices);
gboolean      gts_segment_is_ok                      (GtsSegment * s);

/* Edges: edge.c */

/**
 * GTS_IS_EDGE:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsEdge, %FALSE otherwise.
 */
#define GTS_IS_EDGE(obj) (gts_object_is_from_class (obj,\
                                                    gts_edge_class ()))
/**
 * GTS_EDGE:
 * @obj: a descendant of #GtsEdge.
 *
 * Casts @obj to #GtsEdge.
 */
#define GTS_EDGE(obj) GTS_OBJECT_CAST (obj,\
                                       GtsEdge,\
                                       gts_edge_class ())
/**
 * GTS_EDGE_CLASS:
 * @klass: a descendant of #GtsEdgeClass.
 *
 * Casts @klass to #GtsEdgeClass.
 */
#define GTS_EDGE_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                     GtsEdgeClass,\
                                                     gts_edge_class ())

/**
 * GtsEdge:
 * @segment: The parent object.
 * @triangles: List of #GtsTriangle using this edge.
 *
 * The edge object.
 */
struct _GtsEdge {
  GtsSegment segment;

  GSList * triangles;
};

/**
 * GtsEdgeClass:
 * @parent_class: The parent class.
 *
 * The edge class. No virtual functions are associated.
 */
struct _GtsEdgeClass {
  GtsSegmentClass parent_class;
};

GTS_C_VAR
gboolean      gts_allow_floating_edges;

GtsEdgeClass * gts_edge_class                     (void);
GtsEdge *     gts_edge_new                        (GtsEdgeClass * klass,
                                                   GtsVertex * v1,
                                                   GtsVertex * v2);
/**
 * gts_edge_is_unattached:
 * @s: a #GtsEdge.
 *
 * Evaluates to %TRUE if no triangles uses @s as an edge, %FALSE otherwise.
 */
#define       gts_edge_is_unattached(s) ((s)->triangles == NULL ? TRUE : FALSE)
GtsFace *     gts_edge_has_parent_surface         (GtsEdge * e,
                                                   GtsSurface * surface);
GtsFace *     gts_edge_has_any_parent_surface     (GtsEdge * e);
GtsFace *     gts_edge_is_boundary                (GtsEdge * e,
                                                   GtsSurface * surface);
void          gts_edge_replace                    (GtsEdge * e,
                                                   GtsEdge * with);
GSList *      gts_edges_from_vertices             (GSList * vertices,
                                                   GtsSurface * parent);
guint         gts_edge_face_number                (GtsEdge * e,
                                                   GtsSurface * s);
gboolean      gts_edge_collapse_is_valid          (GtsEdge * e);
gboolean      gts_edge_collapse_creates_fold      (GtsEdge * e,
                                                   GtsVertex * v,
                                                   gdouble max);
GtsEdge *     gts_edge_is_duplicate               (GtsEdge * e);
GList *       gts_edges_merge                     (GList * edges);
gboolean      gts_edge_belongs_to_tetrahedron     (GtsEdge * e);
guint         gts_edge_is_contact                 (GtsEdge * e);
void          gts_edge_swap                       (GtsEdge * e,
                                                   GtsSurface * s);
gboolean      gts_edge_manifold_faces             (GtsEdge * e,
                                                   GtsSurface * s,
                                                   GtsFace ** f1,
                                                   GtsFace ** f2);
gboolean      gts_edge_is_compatible              (GtsEdge * e,
                                                   GtsTriangle * t);

/* Triangles: triangle.c */

/**
 * GTS_IS_TRIANGLE:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a descendant of #GtsTriangle, %FALSE otherwise.
 */
#define GTS_IS_TRIANGLE(obj) (gts_object_is_from_class (obj,\
                                                        gts_triangle_class ()))

/**
 * GTS_TRIANGLE:
 * @obj: a descendant of #GtsTriangle.
 *
 * Casts @obj to #GtsTriangle.
 */
#define GTS_TRIANGLE(obj) GTS_OBJECT_CAST (obj,\
                                           GtsTriangle,\
                                           gts_triangle_class ())

/**
 * GTS_TRIANGLE_CLASS:
 * @klass: a descendant of #GtsTriangleClass.
 *
 * Casts @klass to #GtsTriangleClass.
 */
#define GTS_TRIANGLE_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsTriangleClass,\
                                                         gts_triangle_class ())

/**
 * GtsTriangle:
 * @object: Parent object.
 * @e1: First edge.
 * @e2: Second edge.
 * @e3: Third edge.
 *
 * The triangle object.
 */
struct _GtsTriangle {
  GtsObject object;

  GtsEdge * e1;
  GtsEdge * e2;
  GtsEdge * e3;
};

/**
 * GtsTriangleClass:
 * @parent_class: The parent class.
 *
 * The triangle class. No virtual functions are associated.
 */
struct _GtsTriangleClass {
  GtsObjectClass parent_class;
};

GtsTriangleClass * gts_triangle_class        (void);
void        gts_triangle_set                 (GtsTriangle * triangle,
                                              GtsEdge * e1,
                                              GtsEdge * e2,
                                              GtsEdge * e3);
GtsTriangle * gts_triangle_new               (GtsTriangleClass * klass,
                                              GtsEdge * e1,
                                              GtsEdge * e2,
                                              GtsEdge * e3);
/**
 * gts_triangle_vertex:
 * @t: a #GtsTriangle.
 *
 * Evaluates to the #GtsVertex of @t not used by @t->e1.
 */
#define     gts_triangle_vertex(t) (GTS_SEGMENT (GTS_TRIANGLE (t)->e1)->v1 ==\
                                    GTS_SEGMENT (GTS_TRIANGLE (t)->e2)->v1 || \
                                    GTS_SEGMENT (GTS_TRIANGLE (t)->e1)->v2 ==\
                                    GTS_SEGMENT (GTS_TRIANGLE (t)->e2)->v1 ? \
                                    GTS_SEGMENT (GTS_TRIANGLE (t)->e2)->v2 :\
                                    GTS_SEGMENT (GTS_TRIANGLE (t)->e2)->v1)
GtsVertex *   gts_triangle_vertex_opposite  (GtsTriangle * t,
                                             GtsEdge * e);
GtsEdge *     gts_triangle_edge_opposite    (GtsTriangle * t,
                                             GtsVertex * v);
gdouble       gts_triangles_angle           (GtsTriangle * t1,
                                             GtsTriangle * t2);
gboolean      gts_triangles_are_compatible  (GtsTriangle * t1,
                                             GtsTriangle * t2,
                                             GtsEdge * e);
gdouble       gts_triangle_area             (GtsTriangle * t);
gdouble       gts_triangle_perimeter        (GtsTriangle * t);
gdouble       gts_triangle_quality          (GtsTriangle * t);
void          gts_triangle_normal           (GtsTriangle * t,
                                             gdouble * x,
                                             gdouble * y,
                                             gdouble * z);
gdouble       gts_triangle_orientation      (GtsTriangle * t);
void          gts_triangle_revert           (GtsTriangle * t);
GSList *      gts_triangles_from_edges      (GSList * edges);
void          gts_triangle_vertices_edges   (GtsTriangle * t,
                                             GtsEdge * e,
                                             GtsVertex ** v1,
                                             GtsVertex ** v2,
                                             GtsVertex ** v3,
                                             GtsEdge ** e1,
                                             GtsEdge ** e2,
                                             GtsEdge ** e3);
GtsTriangle * gts_triangle_enclosing        (GtsTriangleClass * klass,
                                             GSList * points,
                                             gdouble scale);
guint         gts_triangle_neighbor_number  (GtsTriangle * t);
GSList *      gts_triangle_neighbors        (GtsTriangle * t);
GtsEdge *     gts_triangles_common_edge     (GtsTriangle * t1,
                                             GtsTriangle * t2);
GtsTriangle * gts_triangle_is_duplicate     (GtsTriangle * t);
GtsTriangle * gts_triangle_use_edges        (GtsEdge * e1,
                                             GtsEdge * e2,
                                             GtsEdge * e3);
gboolean      gts_triangle_is_ok            (GtsTriangle * t);
void          gts_triangle_vertices         (GtsTriangle * t,
                                             GtsVertex ** v1,
                                             GtsVertex ** v2,
                                             GtsVertex ** v3);
GtsPoint *    gts_triangle_circumcircle_center (GtsTriangle * t,
                                                GtsPointClass * point_class);
gboolean      gts_triangles_are_folded      (GSList * triangles,
                                             GtsVertex * A, GtsVertex * B,
                                             gdouble max);
GtsObject *   gts_triangle_is_stabbed       (GtsTriangle * t,
                                             GtsPoint * p,
                                             gdouble * orientation);
void          gts_triangle_interpolate_height (GtsTriangle * t,
                                               GtsPoint * p);

/* Faces: face.c */

/**
 * GTS_IS_FACE:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsFace, %FALSE otherwise.
 */
#define GTS_IS_FACE(obj) (gts_object_is_from_class (obj,\
                                                    gts_face_class ()))

/**
 * GTS_FACE:
 * @obj: a descendant of #GtsFace.
 *
 * Casts @obj to #GtsFace.
 */
#define GTS_FACE(obj) GTS_OBJECT_CAST (obj,\
                                       GtsFace,\
                                       gts_face_class ())

/**
 * GTS_FACE_CLASS:
 * @klass: a descendant of #GtsFaceClass.
 *
 * Casts @klass to #GtsFaceClass.
 */
#define GTS_FACE_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                     GtsFaceClass,\
                                                     gts_face_class ())

/**
 * GtsFace:
 * @triangle: The parent object.
 * @surfaces: A list of #GtsSurface using this face.
 *
 * The face object.
 */
struct _GtsFace {
  GtsTriangle triangle;

  GSList * surfaces;
};

/**
 * GtsFaceClass:
 * @parent_class: The parent class.
 *
 * The face class. No virtual functions are associated.
 */
struct _GtsFaceClass {
  GtsTriangleClass parent_class;
};

GTS_C_VAR
gboolean      gts_allow_floating_faces;

GtsFaceClass * gts_face_class                       (void);
GtsFace *     gts_face_new                          (GtsFaceClass * klass,
                                                     GtsEdge * e1,
                                                     GtsEdge * e2,
                                                     GtsEdge * e3);
gboolean      gts_face_has_parent_surface           (GtsFace * f,
                                                     GtsSurface * s);
GSList *      gts_faces_from_edges                  (GSList * edges,
                                                     GtsSurface * s);
guint         gts_face_neighbor_number              (GtsFace * f,
                                                     GtsSurface * s);
GSList *      gts_face_neighbors                    (GtsFace * f,
                                                     GtsSurface * s);
void          gts_face_foreach_neighbor             (GtsFace * f,
                                                     GtsSurface * s,
                                                     GtsFunc func,
                                                     gpointer data);
gboolean      gts_face_is_compatible                (GtsFace * f,
                                                     GtsSurface * s);

/* Matrices: matrix.c */

/**
 * gts_vector_cross:
 * @C: a #GtsVector.
 * @A: another #GtsVector.
 * @B: and another.
 *
 * Given two vectors @A and @B fills @C with the coordinates of the cross-product @A^@B.
 */
#define       gts_vector_cross(C,A,B) ((C)[0] = (A)[1]*(B)[2] - (A)[2]*(B)[1],\
                                       (C)[1] = (A)[2]*(B)[0] - (A)[0]*(B)[2],\
                                       (C)[2] = (A)[0]*(B)[1] - (A)[1]*(B)[0])

/**
 * gts_vector_init:
 * @v: a #GtsVector.
 * @p1: a #GtsPoint.
 * @p2: another #GtsPoint.
 *
 * Given two points @p1 and @p2, fills @v with the coordinates of vector @p1->@p2.
 */
#define       gts_vector_init(v, p1, p2)   ((v)[0] = (p2)->x - (p1)->x,\
                                            (v)[1] = (p2)->y - (p1)->y,\
                                            (v)[2] = (p2)->z - (p1)->z)
/**
 * gts_vector_scalar:
 * @v1: a #GtsVector.
 * @v2: another #GtsVector.
 *
 * Given two vectors @v1 and @v2 evaluates to the scalar product @v1.@v2.
 */
#define       gts_vector_scalar(v1, v2)    ((v1)[0]*(v2)[0] +\
                                            (v1)[1]*(v2)[1] +\
                                            (v1)[2]*(v2)[2])
#define       gts_vector_norm(v)   (sqrt ((v)[0]*(v)[0] +\
                                          (v)[1]*(v)[1] +\
                                          (v)[2]*(v)[2]))
#define       gts_vector_normalize(v) {\
  gdouble __gts_n = gts_vector_norm (v);\
  if (__gts_n > 0.) {\
    (v)[0] /= __gts_n;\
    (v)[1] /= __gts_n;\
    (v)[2] /= __gts_n;\
  }\
}
GtsMatrix * gts_matrix_new (gdouble a00, gdouble a01, gdouble a02, gdouble a03,
                            gdouble a10, gdouble a11, gdouble a12, gdouble a13,
                            gdouble a20, gdouble a21, gdouble a22, gdouble a23,
                            gdouble a30, gdouble a31, gdouble a32, gdouble a33);
void gts_matrix_assign (GtsMatrix * m,
                        gdouble a00, gdouble a01, gdouble a02, gdouble a03,
                        gdouble a10, gdouble a11, gdouble a12, gdouble a13,
                        gdouble a20, gdouble a21, gdouble a22, gdouble a23,
                        gdouble a30, gdouble a31, gdouble a32, gdouble a33);
GtsMatrix *   gts_matrix_projection                  (GtsTriangle * t);
GtsMatrix *   gts_matrix_transpose                   (GtsMatrix * m);
gdouble       gts_matrix_determinant                 (GtsMatrix * m);
GtsMatrix *   gts_matrix_inverse                     (GtsMatrix * m);
GtsMatrix *   gts_matrix3_inverse                    (GtsMatrix * m);
void          gts_matrix_print                       (GtsMatrix * m,
                                                      FILE * fptr);
guint         gts_matrix_compatible_row              (GtsMatrix * A,
                                                      GtsVector b,
                                                      guint n,
                                                      GtsVector A1,
                                                      gdouble b1);
guint         gts_matrix_quadratic_optimization      (GtsMatrix * A,
                                                      GtsVector b,
                                                      guint n,
                                                      GtsMatrix * H,
                                                      GtsVector c);
GtsMatrix *   gts_matrix_product                     (GtsMatrix * m1,
                                                      GtsMatrix * m2);
GtsMatrix *   gts_matrix_zero                        (GtsMatrix * m);
GtsMatrix *   gts_matrix_identity                    (GtsMatrix * m);
GtsMatrix *   gts_matrix_scale                       (GtsMatrix * m,
                                                      GtsVector s);
GtsMatrix *   gts_matrix_translate                   (GtsMatrix * m,
                                                      GtsVector t);
GtsMatrix *   gts_matrix_rotate                      (GtsMatrix * m,
                                                      GtsVector r,
                                                      gdouble angle);
void          gts_matrix_destroy                     (GtsMatrix * m);
void          gts_vector_print                       (GtsVector v,
                                                      FILE * fptr);
void          gts_vector4_print                      (GtsVector4 v,
                                                      FILE * fptr);

/* Kdtrees: kdtree.c */

#define       gts_kdtree_destroy(tree)               g_node_destroy(tree)

GNode *       gts_kdtree_new                         (GPtrArray * points,
                                                      int (*compare)
                                                      (const void *,
                                                       const void *));
GSList *      gts_kdtree_range                       (GNode * tree,
                                                      GtsBBox * bbox,
                                                      int (*compare)
                                                      (const void *,
                                                      const void *));

/* Bboxtrees: bbtree.c */

/**
 * GtsBBTreeTraverseFunc:
 * @bb1: a #GtsBBox.
 * @bb2: another #GtsBBox.
 * @data: user data passed to the function.
 *
 * User function called for each pair of overlapping bounding
 * boxes. See gts_bb_tree_traverse_overlapping().
 */
typedef void   (*GtsBBTreeTraverseFunc)          (GtsBBox * bb1,
                                                  GtsBBox * bb2,
                                                  gpointer data);
/**
 * GtsBBoxDistFunc:
 * @p: a #GtsPoint.
 * @bounded: an object bounded by a #GtsBBox.
 *
 * User function returning the (minimum) distance between the object
 * defined by @bounded and point @p.
 *
 * Returns: the distance between @p and @bounded.
 */
typedef gdouble (*GtsBBoxDistFunc)               (GtsPoint * p,
                                                  gpointer bounded);
/**
 * GtsBBoxClosestFunc:
 * @p: a #GtsPoint.
 * @bounded: an object bounded by a #GtsBBox.
 *
 * User function returning a #GtsPoint belonging to the object defined
 * by @bounded and closest to @p.
 *
 * Returns: a #GtsPoint.
 */
typedef GtsPoint * (*GtsBBoxClosestFunc)         (GtsPoint * p,
                                                  gpointer bounded);

/**
 * GTS_IS_BBOX:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsBBox, %FALSE otherwise.
 */
#define GTS_IS_BBOX(obj) (gts_object_is_from_class (obj,\
                                                    gts_bbox_class ()))
/**
 * GTS_BBOX:
 * @obj: a descendant of #GtsBBox.
 *
 * Casts @obj to #GtsBBox.
 */
#define GTS_BBOX(obj) GTS_OBJECT_CAST (obj,\
                                       GtsBBox,\
                                       gts_bbox_class ())
/**
 * GTS_BBOX_CLASS:
 * @klass: a descendant of #GtsBBoxClass.
 *
 * Casts @klass to #GtsBBoxClass.
 */
#define GTS_BBOX_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                     GtsBBoxClass,\
                                                     gts_bbox_class ())

/**
 * GtsBBox:
 * @object: The parent object.
 * @bounded: The bounded object.
 * @x1: x coordinate of the lower-left front corner.
 * @y1: y coordinate of the lower-left front corner.
 * @z1: z coordinate of the lower-left front corner.
 * @x2: x coordinate of the upper-right back corner.
 * @y2: y coordinate of the upper-right back corner.
 * @z2: z coordinate of the upper-right back corner.
 *
 * The bounding box object.
 */
struct _GtsBBox {
  GtsObject object;
  gpointer bounded;
  gdouble x1, y1, z1;
  gdouble x2, y2, z2;
};

/**
 * GtsBBoxClass:
 * @parent_class: The parent class.
 *
 * The bounding box class. No virtual functions are associated.
 */
struct _GtsBBoxClass {
  GtsObjectClass parent_class;
};

GtsBBoxClass * gts_bbox_class                (void);
GtsBBox *  gts_bbox_new                      (GtsBBoxClass * klass,
                                              gpointer bounded,
                                              gdouble x1,
                                              gdouble y1,
                                              gdouble z1,
                                              gdouble x2,
                                              gdouble y2,
                                              gdouble z2);
void       gts_bbox_set                      (GtsBBox * bbox,
                                              gpointer bounded,
                                              gdouble x1,
                                              gdouble y1,
                                              gdouble z1,
                                              gdouble x2,
                                              gdouble y2,
                                              gdouble z2);
GtsBBox *  gts_bbox_segment                  (GtsBBoxClass * klass,
                                              GtsSegment * s);
GtsBBox *  gts_bbox_triangle                 (GtsBBoxClass * klass,
                                              GtsTriangle * t);
GtsBBox *  gts_bbox_surface                  (GtsBBoxClass * klass,
                                              GtsSurface * surface);
GtsBBox *  gts_bbox_bboxes                   (GtsBBoxClass * klass,
                                              GSList * bboxes);
GtsBBox *  gts_bbox_points                   (GtsBBoxClass * klass,
                                              GSList * points);
/**
 * gts_bbox_point_is_inside:
 * @bbox: a #GtsBBox.
 * @p: a #GtsPoint.
 *
 * Evaluates to %TRUE if @p is inside (or on the boundary) of @bbox, %FALSE otherwise.
 */
#define    gts_bbox_point_is_inside(bbox, p) ((p)->x >= (bbox)->x1 &&\
                                             (p)->y >= (bbox)->y1 &&\
                                             (p)->z >= (bbox)->z1 &&\
                                             (p)->x <= (bbox)->x2 &&\
                                             (p)->y <= (bbox)->y2 &&\
                                             (p)->z <= (bbox)->z2)
gboolean   gts_bboxes_are_overlapping        (GtsBBox * bb1,
                                              GtsBBox * bb2);
void       gts_bbox_draw                     (GtsBBox * bb,
                                              FILE * fptr);
gdouble    gts_bbox_diagonal2                (GtsBBox * bb);
void       gts_bbox_point_distance2          (GtsBBox * bb,
                                              GtsPoint * p,
                                              gdouble * min,
                                              gdouble * max);
gboolean   gts_bbox_is_stabbed               (GtsBBox * bb,
                                              GtsPoint * p);
gboolean   gts_bbox_overlaps_triangle        (GtsBBox * bb,
                                              GtsTriangle * t);
gboolean   gts_bbox_overlaps_segment         (GtsBBox * bb,
                                              GtsSegment * s);

GNode *    gts_bb_tree_new                   (GSList * bboxes);
GNode *    gts_bb_tree_surface               (GtsSurface * s);
GSList *   gts_bb_tree_stabbed               (GNode * tree,
                                              GtsPoint * p);
GSList *   gts_bb_tree_overlap               (GNode * tree,
                                              GtsBBox * bbox);
gboolean   gts_bb_tree_is_overlapping        (GNode * tree,
                                              GtsBBox * bbox);
void       gts_bb_tree_traverse_overlapping  (GNode * tree1,
                                              GNode * tree2,
                                              GtsBBTreeTraverseFunc func,
                                              gpointer data);
void       gts_bb_tree_draw                  (GNode * tree,
                                              guint depth,
                                              FILE * fptr);
GSList *   gts_bb_tree_point_closest_bboxes  (GNode * tree,
                                              GtsPoint * p);
gdouble    gts_bb_tree_point_distance        (GNode * tree,
                                              GtsPoint * p,
                                              GtsBBoxDistFunc distance,
                                              GtsBBox ** bbox);
GtsPoint * gts_bb_tree_point_closest         (GNode * tree,
                                              GtsPoint * p,
                                              GtsBBoxClosestFunc closest,
                                              gdouble * distance);
void       gts_bb_tree_segment_distance      (GNode * tree,
                                              GtsSegment * s,
                                              GtsBBoxDistFunc distance,
                                              gdouble delta,
                                              GtsRange * range);
void       gts_bb_tree_triangle_distance     (GNode * tree,
                                              GtsTriangle * t,
                                              GtsBBoxDistFunc distance,
                                              gdouble delta,
                                              GtsRange * range);
void       gts_bb_tree_surface_distance      (GNode * tree,
                                              GtsSurface * s,
                                              GtsBBoxDistFunc distance,
                                              gdouble delta,
                                              GtsRange * range);
void       gts_bb_tree_surface_boundary_distance
                                             (GNode * tree,
                                              GtsSurface * s,
                                              GtsBBoxDistFunc distance,
                                              gdouble delta,
                                              GtsRange * range);
void       gts_bb_tree_destroy               (GNode * tree,
                                              gboolean free_leaves);

/* Surfaces: surface.c */

typedef struct _GtsSurfaceStats        GtsSurfaceStats;
typedef struct _GtsSurfaceQualityStats GtsSurfaceQualityStats;
typedef GtsVertex * (*GtsRefineFunc)   (GtsEdge * e,
                                        GtsVertexClass * klass,
                                        gpointer data);
typedef GtsVertex * (*GtsCoarsenFunc)  (GtsEdge * e,
                                        GtsVertexClass * klass,
                                        gpointer data);
typedef gboolean    (*GtsStopFunc)     (gdouble cost,
                                        guint nedge,
                                        gpointer data);

struct _GtsSurfaceStats {
  guint n_faces;
  guint n_incompatible_faces;
  guint n_duplicate_faces;
  guint n_duplicate_edges;
  guint n_boundary_edges;
  guint n_non_manifold_edges;
  GtsRange edges_per_vertex, faces_per_edge;
  GtsSurface * parent;
};

struct _GtsSurfaceQualityStats {
  GtsRange face_quality;
  GtsRange face_area;
  GtsRange edge_length;
  GtsRange edge_angle;
  GtsSurface * parent;
};

/**
 * GtsSurface:
 * @object: The parent object.
 * @faces: a tree data structure of faces.
 * @faces: a tree data structure of faces.
 * @face_class: The face class.
 * @edge_class: The edge class.
 * @vertex_class: The vertex class.
 * @keep_faces: Used internally.
 *
 * The surface object.
 */
struct _GtsSurface {
  GtsObject object;

#ifdef USE_SURFACE_BTREE
  GTree * faces;
#else /* not USE_SURFACE_BTREE */
  GHashTable * faces;
#endif /* not USE_SURFACE_BTREE */
  GtsFaceClass * face_class;
  GtsEdgeClass * edge_class;
  GtsVertexClass * vertex_class;
  gboolean keep_faces;
};

/**
 * GtsSurfaceClass:
 * @parent_class: The parent class.
 *
 * The surface class. No virtual functions is defined.
 */
struct _GtsSurfaceClass {
  GtsObjectClass parent_class;

  void (* add_face)    (GtsSurface *, GtsFace *);
  void (* remove_face) (GtsSurface *, GtsFace *);
};

/**
 * GTS_IS_SURFACE:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsSurface, %FALSE otherwise.
 */
#define GTS_IS_SURFACE(obj) (gts_object_is_from_class (obj,\
                                                       gts_surface_class ()))

/**
 * GTS_SURFACE:
 * @obj: a descendant of #GtsSurface.
 *
 * Casts @obj to #GtsSurface.
 */
#define GTS_SURFACE(obj) GTS_OBJECT_CAST (obj,\
                                          GtsSurface,\
                                          gts_surface_class ())

/**
 * GTS_SURFACE_CLASS:
 * @klass: a descendant of #GtsSurfaceClass.
 *
 * Casts @klass to #GtsSurfaceClass.
 */
#define GTS_SURFACE_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                        GtsSurfaceClass,\
                                                        gts_surface_class ())

GtsSurfaceClass * gts_surface_class        (void);
GtsSurface * gts_surface_new               (GtsSurfaceClass * klass,
                                            GtsFaceClass * face_class,
                                            GtsEdgeClass * edge_class,
                                            GtsVertexClass * vertex_class);
void         gts_surface_add_face          (GtsSurface * s,
                                            GtsFace * f);
void         gts_surface_remove_face       (GtsSurface * s,
                                            GtsFace * f);
guint        gts_surface_read              (GtsSurface * surface,
                                            GtsFile * f);
guint        gts_surface_read_obj          (GtsSurface * surface,
                                            GtsFile * f);
gdouble      gts_surface_area              (GtsSurface * s);
void         gts_surface_stats             (GtsSurface * s,
                                            GtsSurfaceStats * stats);
void         gts_surface_quality_stats     (GtsSurface * s,
                                            GtsSurfaceQualityStats * stats);
void         gts_surface_print_stats       (GtsSurface * s,
                                            FILE * fptr);
void         gts_surface_write             (GtsSurface * s,
                                            FILE * fptr);
void         gts_surface_write_obj         (GtsSurface * s,
                                            FILE * fptr);
void         gts_surface_write_oogl        (GtsSurface * s,
                                            FILE * fptr);
void         gts_surface_write_vtk         (GtsSurface * s,
                                            FILE * fptr);
void         gts_surface_write_oogl_boundary (GtsSurface * s,
                                              FILE * fptr);
void         gts_surface_foreach_vertex    (GtsSurface * s,
                                            GtsFunc func,
                                            gpointer data);
void         gts_surface_foreach_edge      (GtsSurface * s,
                                            GtsFunc func,
                                            gpointer data);
void         gts_surface_foreach_face      (GtsSurface * s,
                                            GtsFunc func,
                                            gpointer data);
guint        gts_surface_foreach_face_remove (GtsSurface * s,
                                              GtsFunc func,
                                              gpointer data);
typedef struct _GtsSurfaceTraverse GtsSurfaceTraverse;
GtsSurfaceTraverse * gts_surface_traverse_new (GtsSurface * s,
                                               GtsFace * f);
GtsFace *    gts_surface_traverse_next     (GtsSurfaceTraverse * t,
                                            guint * level);
void         gts_surface_traverse_destroy  (GtsSurfaceTraverse * t);
void         gts_surface_refine            (GtsSurface * surface,
                                            GtsKeyFunc cost_func,
                                            gpointer cost_data,
                                            GtsRefineFunc refine_func,
                                            gpointer refine_data,
                                            GtsStopFunc stop_func,
                                            gpointer stop_data);
gboolean     gts_edge_collapse_is_valid    (GtsEdge * e);
void         gts_surface_coarsen           (GtsSurface * surface,
                                            GtsKeyFunc cost_func,
                                            gpointer cost_data,
                                            GtsCoarsenFunc coarsen_func,
                                            gpointer coarsen_data,
                                            GtsStopFunc stop_func,
                                            gpointer stop_data,
                                            gdouble minangle);
gboolean     gts_coarsen_stop_number       (gdouble cost,
                                            guint nedge,
                                            guint * min_number);
gboolean     gts_coarsen_stop_cost         (gdouble cost,
                                            guint nedge,
                                            gdouble * max_cost);
void         gts_surface_tessellate        (GtsSurface * s,
                                            GtsRefineFunc refine_func,
                                            gpointer refine_data);
GtsSurface * gts_surface_generate_sphere   (GtsSurface * s,
                                            guint geodesation_order);
GtsSurface * gts_surface_copy              (GtsSurface * s1,
                                            GtsSurface * s2);
void         gts_surface_merge             (GtsSurface * s,
                                            GtsSurface * with);
gboolean     gts_surface_is_manifold       (GtsSurface * s);
gboolean     gts_surface_is_closed         (GtsSurface * s);
gboolean     gts_surface_is_orientable     (GtsSurface * s);
gdouble      gts_surface_volume            (GtsSurface * s);
gdouble      gts_surface_center_of_mass    (GtsSurface * s,
                                            GtsVector cm);
gdouble      gts_surface_center_of_area    (GtsSurface * s,
                                            GtsVector cm);
guint        gts_surface_vertex_number     (GtsSurface * s);
guint        gts_surface_edge_number       (GtsSurface * s);
guint        gts_surface_face_number       (GtsSurface * s);
void         gts_surface_distance          (GtsSurface * s1,
                                            GtsSurface * s2,
                                            gdouble delta,
                                            GtsRange * face_range,
                                            GtsRange * boundary_range);
GSList *     gts_surface_boundary          (GtsSurface * surface);
GSList *     gts_surface_split             (GtsSurface * s);

/* Discrete differential operators: curvature.c */

gboolean gts_vertex_mean_curvature_normal  (GtsVertex * v,
                                            GtsSurface * s,
                                            GtsVector Kh);
gboolean gts_vertex_gaussian_curvature     (GtsVertex * v,
                                            GtsSurface * s,
                                            gdouble * Kg);
void     gts_vertex_principal_curvatures   (gdouble Kh,
                                            gdouble Kg,
                                            gdouble * K1,
                                            gdouble * K2);
void     gts_vertex_principal_directions   (GtsVertex * v,
                                            GtsSurface * s,
                                            GtsVector Kh,
                                            gdouble Kg,
                                            GtsVector e1,
                                            GtsVector e2);

/* Volume optimization: vopt.c */
typedef struct _GtsVolumeOptimizedParams   GtsVolumeOptimizedParams;

struct _GtsVolumeOptimizedParams {
  gdouble volume_weight;
  gdouble boundary_weight;
  gdouble shape_weight;
};

GtsVertex *  gts_volume_optimized_vertex   (GtsEdge * edge,
                                            GtsVertexClass * klass,
                                            GtsVolumeOptimizedParams * params);
gdouble      gts_volume_optimized_cost     (GtsEdge * e,
                                            GtsVolumeOptimizedParams * params);

/* Boolean operations: boolean.c */

GSList *     gts_surface_intersection      (GtsSurface * s1,
                                            GtsSurface * s2,
                                            GNode * faces_tree1,
                                            GNode * faces_tree2);

typedef struct _GtsSurfaceInter         GtsSurfaceInter;
typedef struct _GtsSurfaceInterClass    GtsSurfaceInterClass;

/* begin new functions */
enum _Order {
  S1S2, S2S1, S1S1, S2S2
};
typedef enum _Order Order;

enum _VertexInterType {
  N0NE,
  S0E0, S0E1, S0E2,
  S1E0, S1E1, S1E2,
  S2E3
};
typedef enum _VertexInterType VertexInterType;

typedef struct _VertexInter VertexInter;

struct _VertexInter {
  GtsVertex parent;

  GtsObject * obj1, * obj2;
  VertexInterType type;
};

#define VERTEX_INTER(obj) GTS_OBJECT_CAST (obj,\
                                           VertexInter,\
                                           vertex_inter_class ())
#define IS_VERTEX_INTER(obj) (gts_object_is_from_class (obj,\
                                                        vertex_inter_class ()))

GtsVertexClass * vertex_inter_class (void);
VertexInter * vertex_inter_new (gdouble x, gdouble y, gdouble z,
                                GtsObject * obj1, GtsObject * obj2,
                                VertexInterType type);
void vertex_inter_replace (VertexInter * v, VertexInter * with);

VertexInter * triangle_segment_intersection (GtsTriangle * t,
                                             GtsSegment * s,
                                             Order order);
GSList * intersect_triangles (GtsTriangle * t1, GtsTriangle * t2,
                              GtsSurfaceInter * si);

typedef struct _Orient Orient;
struct _Orient {
    gdouble value;
    gint sign;
    gint sos;
};
/* end new functions */

/**
 * GtsBooleanOperation:
 * @GTS_1_OUT_2: identifies the part of the first surface which lies
 * outside the second surface.
 * @GTS_1_IN_2: identifies the part of the first surface which lies
 * inside the second surface.
 * @GTS_2_OUT_1: identifies the part of the second surface which lies
 * outside the first surface.
 * @GTS_2_IN_1: identifies the part of the second surface which lies
 * inside the first surface.
 *
 * Enum values used for the intersection parts.
 */
typedef enum { GTS_1_OUT_2,
               GTS_1_IN_2,
               GTS_2_OUT_1,
               GTS_2_IN_1 }             GtsBooleanOperation;

/**
 * GTS_IS_SURFACE_INTER:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsSurfaceInter, %FALSE otherwise.
 */
#define GTS_IS_SURFACE_INTER(obj) (gts_object_is_from_class (obj,\
                                        gts_surface_inter_class ()))
/**
 * GTS_SURFACE_INTER:
 * @obj: a descendant of #GtsSurfaceInter.
 *
 * Casts @obj to #GtsSurfaceInter.
 */
#define GTS_SURFACE_INTER(obj) GTS_OBJECT_CAST (obj,\
                                                GtsSurfaceInter,\
                                                gts_surface_inter_class ())
/**
 * GTS_SURFACE_INTER_CLASS:
 * @klass: a descendant of #GtsSurfaceInterClass.
 *
 * Casts @klass to #GtsSurfaceInterClass.
 */
#define GTS_SURFACE_INTER_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                 GtsSurfaceInterClass,\
                                                 gts_surface_inter_class ())

/**
 * GtsSurfaceInter:
 * @object: Parent object.
 * @s1: Local triangulation of the intersecting faces of the first surface.
 * @s2: Local triangulation of the intersecting faces of the second surface.
 * @edges: List of #GtsEdge defining the 3D curve intersection of the two surfaces.
 *
 * The surface intersection object.
 */
struct _GtsSurfaceInter {
  GtsObject object;

  GtsSurface * s1;
  GtsSurface * s2;
  GSList * edges;
};

/**
 * GtsSurfaceInterClass:
 * @parent_class: The parent class.
 *
 * The surface intersection class. No virtual functions are associated.
 */
struct _GtsSurfaceInterClass {
  GtsObjectClass parent_class;
};

GtsSurfaceInterClass *
gts_surface_inter_class          (void);
GtsSurfaceInter *
gts_surface_inter_new            (GtsSurfaceInterClass * klass,
                                  GtsSurface * s1,
                                  GtsSurface * s2,
                                  GNode * faces_tree1,
                                  GNode * faces_tree2,
                                  gboolean is_open1,
                                  gboolean is_open2);
gboolean
gts_surface_inter_check          (GtsSurfaceInter * si,
                                  gboolean * closed);
void
gts_surface_inter_boolean        (GtsSurfaceInter * si,
                                  GtsSurface * surface,
                                  GtsBooleanOperation op);
gboolean gts_surface_foreach_intersecting_face (GtsSurface * s,
                                            GtsBBTreeTraverseFunc func,
                                            gpointer data);
GtsSurface *
gts_surface_is_self_intersecting (GtsSurface * s);

/* Binary Heap: heap.c */

typedef struct _GtsHeap          GtsHeap;

/**
 * GtsHeap:
 *
 * An opaque data structure describing an extended binary heap.
 */

GtsHeap *    gts_heap_new          (GCompareFunc compare_func);
void         gts_heap_insert       (GtsHeap * heap, gpointer p);
gpointer     gts_heap_remove_top   (GtsHeap * heap);
gpointer     gts_heap_top          (GtsHeap * heap);
void         gts_heap_thaw         (GtsHeap * heap);
void         gts_heap_foreach      (GtsHeap * heap,
                                    GFunc func,
                                    gpointer user_data);
void         gts_heap_freeze       (GtsHeap * heap);
guint        gts_heap_size         (GtsHeap * heap);
void         gts_heap_destroy      (GtsHeap * heap);

/* Extended Binary Heap: eheap.c */

typedef struct _GtsEHeap         GtsEHeap;
typedef struct _GtsEHeapPair     GtsEHeapPair;

/**
 * GtsEHeap:
 *
 * An opaque data structure describing an extended binary heap.
 */

/**
 * GtsEHeapPair:
 * @data: Points to the item stored in the heap.
 * @key: Value of the key for this item.
 * @pos: Private field.
 */
struct _GtsEHeapPair {
  gpointer data;
  gdouble key;
  guint pos;
};

GtsEHeap *     gts_eheap_new          (GtsKeyFunc key_func,
                                       gpointer data);
GtsEHeapPair * gts_eheap_insert       (GtsEHeap * heap,
                                       gpointer p);
GtsEHeapPair * gts_eheap_insert_with_key (GtsEHeap * heap,
                                          gpointer p,
                                          gdouble key);
gpointer       gts_eheap_remove_top   (GtsEHeap * heap,
                                       gdouble * key);
gpointer       gts_eheap_top          (GtsEHeap * heap,
                                       gdouble * key);
void           gts_eheap_thaw         (GtsEHeap * heap);
void           gts_eheap_foreach      (GtsEHeap * heap,
                                       GFunc func,
                                       gpointer data);
gpointer       gts_eheap_remove       (GtsEHeap * heap,
                                       GtsEHeapPair * p);
void           gts_eheap_decrease_key (GtsEHeap * heap,
                                       GtsEHeapPair * p,
                                       gdouble new_key);
void           gts_eheap_freeze       (GtsEHeap * heap);
guint          gts_eheap_size         (GtsEHeap * heap);
void           gts_eheap_update       (GtsEHeap * heap);
gdouble        gts_eheap_key          (GtsEHeap * heap,
                                       gpointer p);
void           gts_eheap_randomized   (GtsEHeap * heap,
                                       gboolean randomized);
void           gts_eheap_destroy      (GtsEHeap * heap);

/* FIFO queues: fifo.c */

typedef struct _GtsFifo GtsFifo;

GtsFifo *      gts_fifo_new           (void);
void           gts_fifo_write         (GtsFifo * fifo,
                                       FILE * fp);
void           gts_fifo_push          (GtsFifo * fifo,
                                       gpointer data);
gpointer       gts_fifo_pop           (GtsFifo * fifo);
gpointer       gts_fifo_top           (GtsFifo * fifo);
guint          gts_fifo_size          (GtsFifo * fifo);
gboolean       gts_fifo_is_empty      (GtsFifo * fifo);
void           gts_fifo_foreach       (GtsFifo * fifo,
                                       GtsFunc func,
                                       gpointer data);
void           gts_fifo_reverse       (GtsFifo * fifo);
void           gts_fifo_destroy       (GtsFifo * fifo);

/* Progressive surfaces */

/* split.c */

typedef struct _GtsSplit      GtsSplit;
typedef struct _GtsSplitClass GtsSplitClass;
typedef struct _GtsSplitCFace GtsSplitCFace;

struct _GtsSplit {
  GtsObject object;

  GtsVertex * v;
  GtsObject * v1;
  GtsObject * v2;
  GtsSplitCFace * cfaces;
  guint ncf;
};

struct _GtsSplitClass {
  GtsObjectClass parent_class;
};

#define GTS_IS_SPLIT(obj)    (gts_object_is_from_class (obj,\
                                                        gts_split_class ()))
#define GTS_SPLIT(obj)              GTS_OBJECT_CAST (obj,\
                                                     GtsSplit,\
                                                     gts_split_class ())
#define GTS_SPLIT_CLASS(klass)      GTS_OBJECT_CLASS_CAST (klass,\
                                                     GtsSplitClass,\
                                                     gts_split_class ())
#define GTS_SPLIT_V1(vs)            (GTS_IS_SPLIT ((vs)->v1) ?\
                                     GTS_SPLIT ((vs)->v1)->v :\
                                     GTS_VERTEX ((vs)->v1))
#define GTS_SPLIT_V2(vs)            (GTS_IS_SPLIT ((vs)->v2) ?\
                                     GTS_SPLIT ((vs)->v2)->v :\
                                     GTS_VERTEX ((vs)->v2))

GtsSplitClass *  gts_split_class          (void);
GtsSplit *       gts_split_new            (GtsSplitClass * klass,
                                           GtsVertex * v,
                                           GtsObject * o1,
                                           GtsObject * o2);
void             gts_split_collapse       (GtsSplit * vs,
                                           GtsEdgeClass * klass,
                                           GtsEHeap * heap);
void             gts_split_expand         (GtsSplit * vs,
                                           GtsSurface * s,
                                           GtsEdgeClass * klass);
typedef gboolean (*GtsSplitTraverseFunc)  (GtsSplit * vs,
                                           gpointer data);
void             gts_split_traverse       (GtsSplit * root,
                                           GTraverseType        order,
                                           gint                 depth,
                                           GtsSplitTraverseFunc func,
                                           gpointer             data);
guint            gts_split_height         (GtsSplit * root);

/* psurface.c */

typedef struct _GtsPSurface         GtsPSurface;
typedef struct _GtsPSurfaceClass    GtsPSurfaceClass;

struct _GtsPSurface {
  GtsObject object;

  GtsSurface * s;
  GPtrArray * split;
  GtsSplitClass * split_class;
  guint pos, min;

  GPtrArray * vertices;
  GPtrArray * faces;
};

struct _GtsPSurfaceClass {
  GtsObjectClass parent_class;
};

#define GTS_IS_PSURFACE(obj) (gts_object_is_from_class (obj,\
                                                        gts_psurface_class ()))
#define GTS_PSURFACE(obj)           GTS_OBJECT_CAST (obj,\
                                                     GtsPSurface,\
                                                     gts_psurface_class ())
#define GTS_PSURFACE_CLASS(klass)     GTS_OBJECT_CLASS_CAST (klass,\
                                                     GtsPSurfaceClass,\
                                                     gts_psurface_class ())
#define GTS_PSURFACE_IS_CLOSED(ps)  (!(ps)->vertices)

GtsPSurfaceClass * gts_psurface_class         (void);
GtsPSurface * gts_psurface_new                (GtsPSurfaceClass * klass,
                                               GtsSurface * surface,
                                               GtsSplitClass * split_class,
                                               GtsKeyFunc cost_func,
                                               gpointer cost_data,
                                               GtsCoarsenFunc coarsen_func,
                                               gpointer coarsen_data,
                                               GtsStopFunc stop_func,
                                               gpointer stop_data,
                                               gdouble minangle);
GtsSplit *    gts_psurface_add_vertex         (GtsPSurface * ps);
GtsSplit *    gts_psurface_remove_vertex      (GtsPSurface * ps);
guint         gts_psurface_max_vertex_number  (GtsPSurface * ps);
guint         gts_psurface_min_vertex_number  (GtsPSurface * ps);
void          gts_psurface_set_vertex_number  (GtsPSurface * ps,
                                               guint n);
guint         gts_psurface_get_vertex_number  (GtsPSurface * ps);
void          gts_psurface_write              (GtsPSurface * ps,
                                               FILE * fptr);
GtsPSurface * gts_psurface_open               (GtsPSurfaceClass * klass,
                                               GtsSurface * s,
                                               GtsSplitClass * split_class,
                                               GtsFile * f);
GtsSplit *    gts_psurface_read_vertex        (GtsPSurface * ps,
                                               GtsFile * fp);
void          gts_psurface_close              (GtsPSurface * ps);
void          gts_psurface_foreach_vertex     (GtsPSurface * ps,
                                               GtsFunc func,
                                               gpointer data);

/* hsurface.c */

typedef struct _GtsHSplit        GtsHSplit;
typedef struct _GtsHSplitClass   GtsHSplitClass;
typedef struct _GtsHSurface      GtsHSurface;
typedef struct _GtsHSurfaceClass GtsHSurfaceClass;

/**
 * GtsHSplit:
 * @split: The parent object.
 * @index: Used when managing priority heaps for vertex split and collapse.
 * @parent: Parent of this hierarchical split vertex in the split vertex tree.
 * @nchild: Number of expanded children of this vertex split (0, 1, or 2).
 *
 * The hierarchical split vertex object.
 */
struct _GtsHSplit {
  GtsSplit split;

  GtsEHeapPair * index;
  GtsHSplit * parent;
  guint nchild;
};

/**
 * GtsHSplitClass:
 * @parent_class: a parent class.
 *
 * The hierarchical split vertex class derived from #GtsSplitClass.
 */
struct _GtsHSplitClass {
  GtsSplitClass parent_class;
};

/**
 * GTS_IS_HSPLIT:
 * @obj: a pointer to test.
 *
 * Evaluates to %TRUE if @obj is a #GtsHSplit, %FALSE otherwise.
 */
#define GTS_IS_HSPLIT(obj) (gts_object_is_from_class (obj,\
                                                      gts_hsplit_class ()))
/**
 * GTS_HSPLIT:
 * @obj: a descendant of #GtsHSplit.
 *
 * Casts @obj to #GtsHSplit.
 */
#define GTS_HSPLIT(obj) GTS_OBJECT_CAST (obj,\
                                         GtsHSplit,\
                                         gts_hsplit_class ())

/**
 * GTS_HSPLIT_CLASS:
 * @klass: a descendant of #GtsHSplitClass.
 *
 * Casts @klass to #GtsHSplitClass.
 */
#define GTS_HSPLIT_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                       GtsHSplitClass,\
                                                       gts_hsplit_class ())

GtsHSplitClass * gts_hsplit_class             (void);
GtsHSplit *   gts_hsplit_new                  (GtsHSplitClass * klass,
                                               GtsSplit * vs);
void          gts_hsplit_collapse             (GtsHSplit * hs,
                                               GtsHSurface * hsurface);
void          gts_hsplit_expand               (GtsHSplit * hs,
                                               GtsHSurface * hsurface);
void          gts_hsplit_force_expand         (GtsHSplit * hs,
                                               GtsHSurface * hsurface);

/**
 * GtsHSurface:
 * @object: The parent object.
 * @s: The #GtsSurface being refined or coarsened.
 * @roots: Private field.
 * @expandable: #GtsEHeap of the expandable #GtsHSplit sorted by priority.
 * @collapsable: #GtsEHeap of the collapsable #GtsHSplit sorted by priority.
 * @split: Private field.
 * @nvertex: Private field.
 *
 * The hierarchical surface object.
 */
struct _GtsHSurface {
  GtsObject object;

  GtsSurface * s;
  GSList * roots;
  GtsEHeap * expandable;
  GtsEHeap * collapsable;
  GPtrArray * split;
  guint nvertex;
};

/**
 * GtsHSurfaceClass:
 * @parent_class: The parent class.
 *
 * The hierarchical surface class derived from #GtsObjectClass.
 */
struct _GtsHSurfaceClass {
  GtsObjectClass parent_class;
};

/**
 * GTS_IS_HSURFACE:
 * @obj: a pointer to test.
 *
 * Evaluates to %TRUE if @obj is a #GtsHSurface.
 */
#define GTS_IS_HSURFACE(obj) (gts_object_is_from_class (obj,\
                                                        gts_hsurface_class ()))

/**
 * GTS_HSURFACE:
 * @obj: a descendant of #GtsHSurface.
 *
 * Casts @obj to #GtsHSurface.
 */
#define GTS_HSURFACE(obj) GTS_OBJECT_CAST (obj,\
                                           GtsHSurface,\
                                           gts_hsurface_class ())
/**
 * GTS_HSURFACE_CLASS:
 * @klass: a descendant of #GtsHSurfaceClass.
 *
 * Casts @klass to #GtsHSurfaceClass.
 */
#define GTS_HSURFACE_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsHSurfaceClass,\
                                                         gts_hsurface_class ())

GtsHSurfaceClass * gts_hsurface_class    (void);
GtsHSurface * gts_hsurface_new           (GtsHSurfaceClass * klass,
                                          GtsHSplitClass *   hsplit_class,
                                          GtsPSurface *      psurface,
                                          GtsKeyFunc         expand_key,
                                          gpointer           expand_data,
                                          GtsKeyFunc         collapse_key,
                                          gpointer           collapse_data);
void          gts_hsurface_traverse      (GtsHSurface *        hsurface,
                                          GTraverseType        order,
                                          gint                 depth,
                                          GtsSplitTraverseFunc func,
                                          gpointer             data);
void          gts_hsurface_foreach       (GtsHSurface *        hsurface,
                                          GTraverseType        order,
                                          GtsFunc              func,
                                          gpointer             data);
guint         gts_hsurface_height        (GtsHSurface *        hsurface);

/* Constrained Delaunay triangulation: cdt.c */

/**
 * GTS_IS_CONSTRAINT:
 * @obj: a #GtsObject.
 *
 * Evaluates to %TRUE if @obj is a #GtsConstraint, %FALSE otherwise.
 */
#define GTS_IS_CONSTRAINT(obj) (gts_object_is_from_class (obj,\
                                             gts_constraint_class ()))
/**
 * GTS_CONSTRAINT:
 * @obj: a descendant of #GtsConstraint.
 *
 * Casts @obj to #GtsConstraint.
 */
#define GTS_CONSTRAINT(obj) GTS_OBJECT_CAST (obj,\
                                             GtsConstraint,\
                                             gts_constraint_class ())
/**
 * GTS_CONSTRAINT_CLASS:
 * @klass: a desscendant of #GtsConstraintClass.
 *
 * Casts @klass to #GtsConstraintClass.
 */
#define GTS_CONSTRAINT_CLASS(klass) GTS_OBJECT_CLASS_CAST (klass,\
                                             GtsConstraintClass,\
                                             gts_constraint_class ())

typedef struct _GtsConstraint        GtsConstraint;
typedef struct _GtsConstraintClass   GtsConstraintClass;

GtsConstraintClass * gts_constraint_class        (void);

GtsFace *            gts_point_locate            (GtsPoint * p,
                                                  GtsSurface * surface,
                                                  GtsFace * guess);
GtsVertex *          gts_delaunay_add_vertex_to_face (GtsSurface * surface,
                                                      GtsVertex * v,
                                                      GtsFace * f);
GtsVertex *          gts_delaunay_add_vertex     (GtsSurface * surface,
                                                  GtsVertex * v,
                                                  GtsFace * guess);
void                 gts_delaunay_remove_vertex  (GtsSurface * surface,
                                                  GtsVertex * v);
GtsFace *            gts_delaunay_check          (GtsSurface * surface);
GSList *             gts_delaunay_add_constraint (GtsSurface * surface,
                                                  GtsConstraint * c);
void                 gts_delaunay_remove_hull    (GtsSurface * surface);

/* GtsListFace: Header */

typedef struct _GtsListFace         GtsListFace;

struct _GtsListFace {
  /*< private >*/
  GtsFace parent;

  /*< public >*/
  GSList * points;
};

#define GTS_LIST_FACE(obj)            GTS_OBJECT_CAST (obj,\
                                                 GtsListFace,\
                                                 gts_list_face_class ())
#define GTS_IS_LIST_FACE(obj)         (gts_object_is_from_class (obj,\
                                                 gts_list_face_class ()))

GtsFaceClass *       gts_list_face_class         (void);

/* Constrained Delaunay refinement: refine.c */

typedef gboolean   (* GtsEncroachFunc)           (GtsVertex * v,
                                                  GtsEdge * e,
                                                  GtsSurface * s,
                                                  gpointer data);

gboolean             gts_vertex_encroaches_edge  (GtsVertex * v,
                                                  GtsEdge * e);
GtsVertex *          gts_edge_is_encroached      (GtsEdge * e,
                                                  GtsSurface * s,
                                                  GtsEncroachFunc encroaches,
                                                  gpointer data);
guint                gts_delaunay_conform        (GtsSurface * surface,
                                                  gint steiner_max,
                                                  GtsEncroachFunc encroaches,
                                                  gpointer data);
guint                gts_delaunay_refine         (GtsSurface * surface,
                                                  gint steiner_max,
                                                  GtsEncroachFunc encroaches,
                                                  gpointer encroach_data,
                                                  GtsKeyFunc cost,
                                                  gpointer cost_data);

/* Isosurfaces (marching cubes): iso.c */

/**
 * GtsIsoSlice:
 *
 * A GtsIsoSlice is an opaque data structure containing the
 * coordinates of the points located on the isosurface for a given
 * slice of the user-defined function.
 */

typedef struct _GtsGridPlane     GtsGridPlane;
typedef struct _GtsIsoSlice      GtsIsoSlice;
typedef struct _GtsCartesianGrid GtsCartesianGrid;

struct _GtsGridPlane {
  GtsPoint ** p;
  guint nx, ny;
};

/**
 * GtsCartesianGrid:
 * @nx: Number of points in direction x.
 * @ny: Number of points in direction y.
 * @nz: Number of points in direction z.
 * @x: x coordinate of the first point.
 * @dx: Increment in direction x.
 * @y: y coordinate of the first point.
 * @dy: Increment in direction y.
 * @z: z coordinate of the first point.
 * @dz: Increment in direction z.
 *
 * A regular cartesian grid is defined by the number of points in
 * each direction, the spatial increment in each direction and the
 * coordinates of the first point.
 */
struct _GtsCartesianGrid {
  guint nx, ny, nz;
  gdouble x, dx, y, dy, z, dz;
};

/**
 * GtsIsoCartesianFunc:
 * @a: the 2D array to be filled.
 * @g: a #GtsCartesianGrid.
 * @i: the z direction index of the plane to be filled.
 * @data: user data.
 *
 * This user-defined function is passed to the
 * gts_isosurface_cartesian() function. Given a particular
 * #GtsCartesianGrid @g it must fill the array @a with the values of
 * the function for the plane z=@g.z (or for the corresponding index
 * @i). This function is always called in order by
 * gts_isosurface_cartesian(), i.e. @i varies from 0 to @g.nz - 1
 * with an increment of one (and similarly @g.z is incremented every
 * time by @g.dz).
 */
typedef void (*GtsIsoCartesianFunc)         (gdouble ** a,
                                             GtsCartesianGrid g,
                                             guint i,
                                             gpointer data);

GtsGridPlane * gts_grid_plane_new           (guint nx,
                                             guint ny);
void           gts_grid_plane_destroy       (GtsGridPlane * g);
GtsIsoSlice *  gts_iso_slice_new            (guint nx, guint ny);
void           gts_iso_slice_fill           (GtsIsoSlice * slice,
                                             GtsGridPlane * plane1,
                                             GtsGridPlane * plane2,
                                             gdouble ** f1,
                                             gdouble ** f2,
                                             gdouble iso,
                                             GtsVertexClass * klass);
void           gts_iso_slice_fill_cartesian (GtsIsoSlice * slice,
                                             GtsCartesianGrid g,
                                             gdouble ** f1,
                                             gdouble ** f2,
                                             gdouble iso,
                                             GtsVertexClass * klass);
void           gts_iso_slice_destroy        (GtsIsoSlice * slice);
void           gts_isosurface_slice         (GtsIsoSlice * slice1,
                                             GtsIsoSlice * slice2,
                                             GtsSurface * surface);
void           gts_isosurface_cartesian     (GtsSurface * surface,
                                             GtsCartesianGrid g,
                                             GtsIsoCartesianFunc f,
                                             gpointer data,
                                             gdouble iso);

/* Isosurfaces (marching tetrahedra): isotetra.c */

void           gts_isosurface_tetra         (GtsSurface * surface,
                                             GtsCartesianGrid g,
                                             GtsIsoCartesianFunc f,
                                             gpointer data,
                                             gdouble iso);
void           gts_isosurface_tetra_bcl     (GtsSurface * surface,
                                             GtsCartesianGrid g,
                                             GtsIsoCartesianFunc f,
                                             gpointer data,
                                             gdouble iso);
void           gts_isosurface_tetra_bounded (GtsSurface * surface,
                                             GtsCartesianGrid g,
                                             GtsIsoCartesianFunc f,
                                             gpointer data,
                                             gdouble iso);

/* Named vertices, edges and triangles: named.c */

#define GTS_NAME_LENGTH             40

#define GTS_NVERTEX(obj)            GTS_OBJECT_CAST (obj,\
                                                     GtsNVertex,\
                                                     gts_nvertex_class ())
#define GTS_NVERTEX_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                           GtsNVertexClass,\
                                                           gts_nvertex_class())
#define GTS_IS_NVERTEX(obj)         (gts_object_is_from_class (obj,\
                                       gts_nvertex_class ()))

typedef struct _GtsNVertex          GtsNVertex;
typedef struct _GtsNVertexClass     GtsNVertexClass;

struct _GtsNVertex {
  GtsVertex parent;
  char name[GTS_NAME_LENGTH];
};

struct _GtsNVertexClass {
  GtsVertexClass parent_class;
};

GtsNVertexClass * gts_nvertex_class        (void);

#define GTS_NEDGE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsNEdge,\
                                                   gts_nedge_class ())
#define GTS_NEDGE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsNEdgeClass,\
                                                         gts_nedge_class())
#define GTS_IS_NEDGE(obj)         (gts_object_is_from_class (obj,\
                                       gts_nedge_class ()))

typedef struct _GtsNEdge          GtsNEdge;
typedef struct _GtsNEdgeClass     GtsNEdgeClass;

struct _GtsNEdge {
  GtsEdge parent;
  char name[GTS_NAME_LENGTH];
};

struct _GtsNEdgeClass {
  GtsEdgeClass parent_class;
};

GtsNEdgeClass *   gts_nedge_class        (void);

#define GTS_NFACE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsNFace,\
                                                   gts_nface_class ())
#define GTS_NFACE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsNFaceClass,\
                                                         gts_nface_class())
#define GTS_IS_NFACE(obj)         (gts_object_is_from_class (obj,\
                                       gts_nface_class ()))

typedef struct _GtsNFace          GtsNFace;
typedef struct _GtsNFaceClass     GtsNFaceClass;

struct _GtsNFace {
  GtsFace parent;
  char name[GTS_NAME_LENGTH];
};

struct _GtsNFaceClass {
  GtsFaceClass parent_class;
};

GtsNFaceClass *       gts_nface_class        (void);

/* Cluster object for out-of-core simplification: oocs.c */

#define GTS_CLUSTER(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsCluster,\
                                                   gts_cluster_class ())
#define GTS_CLUSTER_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsClusterClass,\
                                                         gts_cluster_class())
#define GTS_IS_CLUSTER(obj)         (gts_object_is_from_class (obj,\
                                                   gts_cluster_class ()))

typedef struct _GtsCluster         GtsCluster;
typedef struct _GtsClusterClass    GtsClusterClass;
typedef struct _GtsClusterId       GtsClusterId;

struct _GtsClusterId {
  guint x, y, z;
};

struct _GtsCluster {
  GtsObject parent;

  GtsClusterId id;
  GtsVertex * v;
  guint n;
};

struct _GtsClusterClass {
  GtsObjectClass parent_class;

  void (* add) (GtsCluster * c, GtsPoint * p, gpointer data);
  void (* update) (GtsCluster * c);
};

GtsClusterClass * gts_cluster_class                (void);
GtsCluster *      gts_cluster_new                  (GtsClusterClass * klass,
                                                    GtsClusterId id,
                                                    GtsVertexClass * vklass);
void              gts_cluster_add                  (GtsCluster * c,
                                                    GtsPoint * p,
                                                    gpointer data);
void              gts_cluster_update               (GtsCluster * c);

/* Cluster group object for out-of-core simplification: oocs.c */

#define GTS_CLUSTER_GRID(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsClusterGrid,\
                                                   gts_cluster_grid_class ())
#define GTS_CLUSTER_GRID_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                   GtsClusterGridClass,\
                                                   gts_cluster_grid_class())
#define GTS_IS_CLUSTER_GRID(obj)         (gts_object_is_from_class (obj,\
                                                   gts_cluster_grid_class ()))

typedef struct _GtsClusterGrid         GtsClusterGrid;
typedef struct _GtsClusterGridClass    GtsClusterGridClass;

struct _GtsClusterGrid {
  GtsObject parent;

  GtsSurface * surface;
  GtsBBox * bbox;
  GtsVector size;

  GtsClusterClass * cluster_class;
  GHashTable * clusters;
};

struct _GtsClusterGridClass {
  GtsObjectClass parent_class;
};

GtsClusterGridClass * gts_cluster_grid_class (void);
GtsClusterGrid *      gts_cluster_grid_new   (GtsClusterGridClass * klass,
                                              GtsClusterClass * cluster_class,
                                              GtsSurface * s,
                                              GtsBBox * bbox,
                                              gdouble delta);
void           gts_cluster_grid_add_triangle (GtsClusterGrid * cluster_grid,
                                              GtsPoint * p1,
                                              GtsPoint * p2,
                                              GtsPoint * p3,
                                              gpointer data);
GtsRange       gts_cluster_grid_update       (GtsClusterGrid * cluster_grid);

/* Triangle strip generation: stripe.c */
GSList *       gts_surface_strip             (GtsSurface * s);

/* GtsContainee: container.c */

typedef struct _GtsContainee         GtsContainee;
typedef struct _GtsContaineeClass    GtsContaineeClass;
typedef struct _GtsContainer         GtsContainer;
typedef struct _GtsContainerClass    GtsContainerClass;

/**
 * GtsContainee:
 * @object: The parent object.
 *
 * The containee object.
 */
struct _GtsContainee {
  GtsObject object;
};

struct _GtsContaineeClass {
  GtsObjectClass parent_class;

  void     (* add_container)    (GtsContainee *, GtsContainer *);
  void     (* remove_container) (GtsContainee *, GtsContainer *);
  void     (* foreach)          (GtsContainee *, GtsFunc, gpointer);
  gboolean (* is_contained)     (GtsContainee *, GtsContainer *);
  void     (* replace)          (GtsContainee *, GtsContainee *);
};

#define GTS_CONTAINEE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsContainee,\
                                                   gts_containee_class ())
#define GTS_CONTAINEE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsContaineeClass,\
                                                         gts_containee_class())
#define GTS_IS_CONTAINEE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_containee_class ()))

GtsContaineeClass * gts_containee_class        (void);
GtsContainee *      gts_containee_new          (GtsContaineeClass * klass);
gboolean            gts_containee_is_contained (GtsContainee * item,
                                                GtsContainer * c);
void                gts_containee_replace      (GtsContainee * item,
                                                GtsContainee * with);

/* GtsSListContainee: container.c */

typedef struct _GtsSListContainee         GtsSListContainee;
typedef struct _GtsSListContaineeClass    GtsSListContaineeClass;

/**
 * GtsSListContainee:
 * @containee: The parent object.
 * @containers: A list of #GtsContainer containing this object.
 *
 * The singly-linked list containee object.
 */
struct _GtsSListContainee {
  GtsContainee containee;

  GSList * containers;
};

struct _GtsSListContaineeClass {
  GtsContaineeClass parent_class;
};

#define GTS_SLIST_CONTAINEE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsSListContainee,\
                                                   gts_slist_containee_class ())
#define GTS_SLIST_CONTAINEE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsSListContaineeClass,\
                                                         gts_slist_containee_class())
#define GTS_IS_SLIST_CONTAINEE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_slist_containee_class ()))

GtsSListContaineeClass * gts_slist_containee_class   (void);

/* GtsContainer: container.c */

/**
 * GtsContainer:
 * @object: The parent object.
 *
 * The container object.
 */
struct _GtsContainer {
  GtsSListContainee object;
};

struct _GtsContainerClass {
  GtsSListContaineeClass parent_class;

  void  (* add)     (GtsContainer *, GtsContainee *);
  void  (* remove)  (GtsContainer *, GtsContainee *);
  void  (* foreach) (GtsContainer *, GtsFunc, gpointer);
  guint (* size)    (GtsContainer *);
};

#define GTS_CONTAINER(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsContainer,\
                                                   gts_container_class ())
#define GTS_CONTAINER_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsContainerClass,\
                                                         gts_container_class())
#define GTS_IS_CONTAINER(obj)         (gts_object_is_from_class (obj,\
                                                   gts_container_class ()))

GtsContainerClass * gts_container_class     (void);
GtsContainer *      gts_container_new       (GtsContainerClass * klass);
void                gts_container_add       (GtsContainer * c,
                                             GtsContainee * item);
void                gts_container_remove    (GtsContainer * c,
                                             GtsContainee * item);
void                gts_container_foreach   (GtsContainer * c,
                                             GtsFunc func,
                                             gpointer data);
guint               gts_container_size      (GtsContainer * c);

/* GtsHashContainer: container.c */

typedef struct _GtsHashContainer         GtsHashContainer;
typedef struct _GtsHashContainerClass    GtsHashContainerClass;

struct _GtsHashContainer {
  GtsContainer c;

  GHashTable * items;
  gboolean frozen;
};

struct _GtsHashContainerClass {
  GtsContainerClass parent_class;
};

#define GTS_HASH_CONTAINER(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsHashContainer,\
                                                   gts_hash_container_class ())
#define GTS_HASH_CONTAINER_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsHashContainerClass,\
                                                         gts_hash_container_class())
#define GTS_IS_HASH_CONTAINER(obj)         (gts_object_is_from_class (obj,\
                                                   gts_hash_container_class ()))

GtsHashContainerClass * gts_hash_container_class (void);

/* GtsSListContainer: container.c */

typedef struct _GtsSListContainer         GtsSListContainer;
typedef struct _GtsSListContainerClass    GtsSListContainerClass;

struct _GtsSListContainer {
  GtsContainer c;

  GSList * items;
  gboolean frozen;
};

struct _GtsSListContainerClass {
  GtsContainerClass parent_class;
};

#define GTS_SLIST_CONTAINER(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsSListContainer,\
                                                   gts_slist_container_class ())
#define GTS_SLIST_CONTAINER_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsSListContainerClass,\
                                                         gts_slist_container_class())
#define GTS_IS_SLIST_CONTAINER(obj)         (gts_object_is_from_class (obj,\
                                                   gts_slist_container_class ()))

GtsSListContainerClass * gts_slist_container_class (void);

/* GtsGNode: graph.c */

typedef struct _GtsGNode         GtsGNode;
typedef struct _GtsGNodeClass    GtsGNodeClass;
typedef struct _GtsGraph         GtsGraph;
typedef struct _GtsGraphClass    GtsGraphClass;

struct _GtsGNode {
  GtsSListContainer container;

  guint level;
};

struct _GtsGNodeClass {
  GtsSListContainerClass parent_class;

  gfloat (* weight) (GtsGNode *);
  void   (* write)  (GtsGNode *, FILE *);
};

#define GTS_GNODE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsGNode,\
                                                   gts_gnode_class ())
#define GTS_GNODE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsGNodeClass,\
                                                         gts_gnode_class())
#define GTS_IS_GNODE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_gnode_class ()))
#define GTS_GNODE_NEIGHBOR(n,e)   (GTS_GEDGE (e)->n1 == n ? GTS_GEDGE (e)->n2 : GTS_GEDGE (e)->n2 == n ? GTS_GEDGE (e)->n1 : NULL)

GtsGNodeClass * gts_gnode_class                (void);
GtsGNode *      gts_gnode_new                  (GtsGNodeClass * klass);
void            gts_gnode_foreach_neighbor     (GtsGNode * n,
                                                GtsGraph * g,
                                                GtsFunc func,
                                                gpointer data);
void            gts_gnode_foreach_edge         (GtsGNode * n,
                                                GtsGraph * g,
                                                GtsFunc func,
                                                gpointer data);
guint           gts_gnode_degree               (GtsGNode * n,
                                                GtsGraph * g);
gfloat          gts_gnode_move_cost            (GtsGNode * n,
                                                GtsGraph * src,
                                                GtsGraph * dst);
gfloat          gts_gnode_weight               (GtsGNode * n);

GTS_C_VAR
gboolean        gts_allow_floating_gnodes;

/* GtsNGNode: graph.c */

typedef struct _GtsNGNode         GtsNGNode;
typedef struct _GtsNGNodeClass    GtsNGNodeClass;

struct _GtsNGNode {
  GtsGNode node;

  guint id;
};

struct _GtsNGNodeClass {
  GtsGNodeClass parent_class;
};

#define GTS_NGNODE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsNGNode,\
                                                   gts_ngnode_class ())
#define GTS_NGNODE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsNGNodeClass,\
                                                         gts_ngnode_class())
#define GTS_IS_NGNODE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_ngnode_class ()))

GtsNGNodeClass * gts_ngnode_class                (void);
GtsNGNode *      gts_ngnode_new                  (GtsNGNodeClass * klass,
                                                  guint id);

/* GtsWGNode: graph.c */

typedef struct _GtsWGNode         GtsWGNode;
typedef struct _GtsWGNodeClass    GtsWGNodeClass;

struct _GtsWGNode {
  GtsGNode node;

  gfloat weight;
};

struct _GtsWGNodeClass {
  GtsGNodeClass parent_class;
};

#define GTS_WGNODE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsWGNode,\
                                                   gts_wgnode_class ())
#define GTS_WGNODE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsWGNodeClass,\
                                                         gts_wgnode_class())
#define GTS_IS_WGNODE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_wgnode_class ()))

GtsWGNodeClass * gts_wgnode_class                (void);
GtsWGNode *      gts_wgnode_new                  (GtsWGNodeClass * klass,
                                                  gfloat weight);

/* GtsPNode */

typedef struct _GtsPNode         GtsPNode;
typedef struct _GtsPNodeClass    GtsPNodeClass;

struct _GtsPNode {
  GtsGNode node;

  gpointer data;
};

struct _GtsPNodeClass {
  GtsGNodeClass parent_class;
};

#define GTS_PNODE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsPNode,\
                                                   gts_pnode_class ())
#define GTS_PNODE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsPNodeClass,\
                                                         gts_pnode_class())
#define GTS_IS_PNODE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_pnode_class ()))

GtsPNodeClass * gts_pnode_class                (void);
GtsPNode *      gts_pnode_new                  (GtsPNodeClass * klass,
                                                gpointer data);

/* GtsFNode */

typedef struct _GtsFNode         GtsFNode;
typedef struct _GtsFNodeClass    GtsFNodeClass;

struct _GtsFNode {
  GtsGNode node;

  GtsFace * f;
};

struct _GtsFNodeClass {
  GtsGNodeClass parent_class;
};

#define GTS_FNODE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsFNode,\
                                                   gts_fnode_class ())
#define GTS_FNODE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsFNodeClass,\
                                                         gts_fnode_class())
#define GTS_IS_FNODE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_fnode_class ()))

GtsFNodeClass * gts_fnode_class                (void);
GtsFNode *      gts_fnode_new                  (GtsFNodeClass * klass,
                                                GtsFace * f);

/* GtsGEdge: graph.c */

typedef struct _GtsGEdge         GtsGEdge;
typedef struct _GtsGEdgeClass    GtsGEdgeClass;

struct _GtsGEdge {
  GtsContainee containee;

  GtsGNode * n1;
  GtsGNode * n2;
};

struct _GtsGEdgeClass {
  GtsContaineeClass parent_class;

  GtsGEdge * (* link)   (GtsGEdge * e, GtsGNode * n1, GtsGNode * n2);
  gfloat     (* weight) (GtsGEdge * e);
  void       (* write)  (GtsGEdge * e, FILE * fp);
};

#define GTS_GEDGE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsGEdge,\
                                                   gts_gedge_class ())
#define GTS_GEDGE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsGEdgeClass,\
                                                         gts_gedge_class())
#define GTS_IS_GEDGE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_gedge_class ()))

GtsGEdgeClass * gts_gedge_class                (void);
GtsGEdge *      gts_gedge_new                  (GtsGEdgeClass * klass,
                                                GtsGNode * n1,
                                                GtsGNode * n2);
gfloat          gts_gedge_weight               (GtsGEdge * e);
#define         gts_gedge_connects(e, a1, a2)\
   (((e)->n1 == a1 && (e)->n2 == a2) || ((e)->n1 == a2 && (e)->n2 == a1))

/* GtsPGEdge: graph.c */

typedef struct _GtsPGEdge         GtsPGEdge;
typedef struct _GtsPGEdgeClass    GtsPGEdgeClass;

struct _GtsPGEdge {
  GtsGEdge gedge;

  gpointer data;
};

struct _GtsPGEdgeClass {
  GtsGEdgeClass parent_class;
};

#define GTS_PGEDGE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsPGEdge,\
                                                   gts_pgedge_class ())
#define GTS_PGEDGE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsPGEdgeClass,\
                                                         gts_pgedge_class())
#define GTS_IS_PGEDGE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_pgedge_class ()))

GtsPGEdgeClass * gts_pgedge_class                (void);
GtsPGEdge *      gts_pgedge_new                  (GtsPGEdgeClass * klass,
                                                  GtsGNode * n1,
                                                  GtsGNode * n2,
                                                  gpointer data);

/* GtsWGEdge: graph.c */

typedef struct _GtsWGEdge         GtsWGEdge;
typedef struct _GtsWGEdgeClass    GtsWGEdgeClass;

struct _GtsWGEdge {
  GtsGEdge gedge;

  gfloat weight;
};

struct _GtsWGEdgeClass {
  GtsGEdgeClass parent_class;
};

#define GTS_WGEDGE(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsWGEdge,\
                                                   gts_wgedge_class ())
#define GTS_WGEDGE_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsWGEdgeClass,\
                                                         gts_wgedge_class())
#define GTS_IS_WGEDGE(obj)         (gts_object_is_from_class (obj,\
                                                   gts_wgedge_class ()))

GtsWGEdgeClass * gts_wgedge_class                (void);
GtsWGEdge *      gts_wgedge_new                  (GtsWGEdgeClass * klass,
                                                  GtsGNode * n1,
                                                  GtsGNode * n2,
                                                  gfloat weight);

/* GtsGraph: graph.c */

struct _GtsGraph {
  GtsHashContainer object;

  GtsGraphClass * graph_class;
  GtsGNodeClass * node_class;
  GtsGEdgeClass * edge_class;
};

struct _GtsGraphClass {
  GtsHashContainerClass parent_class;

  gfloat (* weight) (GtsGraph *);
};

#define GTS_GRAPH(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsGraph,\
                                                   gts_graph_class ())
#define GTS_GRAPH_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsGraphClass,\
                                                         gts_graph_class())
#define GTS_IS_GRAPH(obj)         (gts_object_is_from_class (obj,\
                                                   gts_graph_class ()))

GtsGraphClass * gts_graph_class                  (void);
GtsGraph *      gts_graph_new                    (GtsGraphClass * klass,
                                                  GtsGNodeClass * node_class,
                                                  GtsGEdgeClass * edge_class);
void            gts_graph_print_stats            (GtsGraph * g,
                                                  FILE * fp);
typedef struct _GtsGraphTraverse GtsGraphTraverse;
typedef enum   { GTS_BREADTH_FIRST
               }   GtsTraverseType;
GtsGraphTraverse * gts_graph_traverse_new        (GtsGraph * g,
                                                  GtsGNode * n,
                                                  GtsTraverseType type,
                                                  gboolean reinit);
GtsGNode *         gts_graph_traverse_next       (GtsGraphTraverse * t);
GtsGNode *         gts_graph_traverse_what_next  (GtsGraphTraverse * t);
void               gts_graph_traverse_destroy    (GtsGraphTraverse * t);
void               gts_graph_foreach_edge        (GtsGraph * g,
                                                  GtsFunc func,
                                                  gpointer data);
gfloat             gts_graph_weight              (GtsGraph * g);
guint              gts_graph_distance_sum        (GtsGraph * g,
                                                  GtsGNode * center);
GtsGNode *         gts_graph_farthest            (GtsGraph * g,
                                                  GSList * gnodes);
guint              gts_graph_edges_cut           (GtsGraph * g);
gfloat             gts_graph_edges_cut_weight    (GtsGraph * g);
void               gts_graph_write               (GtsGraph * g,
                                                  FILE * fp);
void               gts_graph_write_dot           (GtsGraph * g,
                                                  FILE * fp);
GtsGraph *         gts_graph_read                (GtsFile * fp);
guint              gts_graph_read_jostle         (GtsGraph * g,
                                                  GtsFile * fp);

/* GtsWGraph: graph.c */

typedef struct _GtsWGraph      GtsWGraph;
typedef struct _GtsWGraphClass GtsWGraphClass;

struct _GtsWGraph {
  GtsGraph graph;

  gfloat weight;
};

struct _GtsWGraphClass {
  GtsGraphClass parent_class;
};

#define GTS_WGRAPH(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsWGraph,\
                                                   gts_wgraph_class ())
#define GTS_WGRAPH_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsWGraphClass,\
                                                         gts_wgraph_class())
#define GTS_IS_WGRAPH(obj)         (gts_object_is_from_class (obj,\
                                                   gts_wgraph_class ()))

GtsWGraphClass * gts_wgraph_class                (void);
gfloat           gts_wgraph_weight_max           (GtsWGraph * wg);

/* Surface graph: graph.c */

GtsGraph *       gts_surface_graph_new           (GtsGraphClass * klass,
                                                  GtsSurface * s);
GtsSurface *     gts_surface_graph_surface       (GtsGraph * surface_graph,
                                                  GtsSurface * s);

/* Segments graph: graph.c */

GtsGraph *       gts_segments_graph_new          (GtsGraphClass * klass,
                                                  GSList * segments);

/* GtsGNodeSplit: pgraph.c */

typedef struct _GtsGNodeSplit         GtsGNodeSplit;
typedef struct _GtsGNodeSplitClass    GtsGNodeSplitClass;

struct _GtsGNodeSplit {
  GtsObject object;

  GtsGNode * n;
  GtsObject * n1;
  GtsObject * n2;
};

struct _GtsGNodeSplitClass {
  GtsObjectClass parent_class;
};

#define GTS_GNODE_SPLIT(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsGNodeSplit,\
                                                   gts_gnode_split_class ())
#define GTS_GNODE_SPLIT_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsGNodeSplitClass,\
                                                         gts_gnode_split_class())
#define GTS_IS_GNODE_SPLIT(obj)         (gts_object_is_from_class (obj,\
                                                   gts_gnode_split_class ()))
#define GTS_GNODE_SPLIT_N1(ns) (GTS_IS_GNODE_SPLIT ((ns)->n1) ? GTS_GNODE_SPLIT ((ns)->n1)->n : GTS_GNODE ((ns)->n1))
#define GTS_GNODE_SPLIT_N2(ns) (GTS_IS_GNODE_SPLIT ((ns)->n2) ? GTS_GNODE_SPLIT ((ns)->n2)->n : GTS_GNODE ((ns)->n2))

GtsGNodeSplitClass * gts_gnode_split_class    (void);
GtsGNodeSplit *      gts_gnode_split_new      (GtsGNodeSplitClass * klass,
                                               GtsGNode * n,
                                               GtsObject * n1,
                                               GtsObject * n2);
void                 gts_gnode_split_collapse (GtsGNodeSplit * ns,
                                               GtsGraph * g,
                                               GtsWGEdgeClass * klass);
void                 gts_gnode_split_expand   (GtsGNodeSplit * ns,
                                               GtsGraph * g);

/* GtsPGraph: pgraph.c */

typedef struct _GtsPGraph         GtsPGraph;
typedef struct _GtsPGraphClass    GtsPGraphClass;

struct _GtsPGraph {
  GtsObject object;

  GtsGraph * g;
  GPtrArray * split;
  GArray * levels;
  GtsGNodeSplitClass * split_class;
  GtsWGEdgeClass * edge_class;
  guint pos, min, level;
};

struct _GtsPGraphClass {
  GtsObjectClass parent_class;
};

#define GTS_PGRAPH(obj)            GTS_OBJECT_CAST (obj,\
                                                   GtsPGraph,\
                                                   gts_pgraph_class ())
#define GTS_PGRAPH_CLASS(klass)    GTS_OBJECT_CLASS_CAST (klass,\
                                                         GtsPGraphClass,\
                                                         gts_pgraph_class())
#define GTS_IS_PGRAPH(obj)         (gts_object_is_from_class (obj,\
                                                   gts_pgraph_class ()))

GtsPGraphClass * gts_pgraph_class            (void);
GtsPGraph *      gts_pgraph_new              (GtsPGraphClass * klass,
                                              GtsGraph * g,
                                              GtsGNodeSplitClass * split_class,
                                              GtsWGNodeClass * node_class,
                                              GtsWGEdgeClass * edge_class,
                                              guint min);
GtsGNodeSplit *  gts_pgraph_add_node         (GtsPGraph * pg);
GtsGNodeSplit *  gts_pgraph_remove_node      (GtsPGraph * pg);
void             gts_pgraph_set_node_number  (GtsPGraph *pg,
                                              guint n);
guint            gts_pgraph_get_node_number  (GtsPGraph *pg);
guint            gts_pgraph_min_node_number  (GtsPGraph *pg);
guint            gts_pgraph_max_node_number  (GtsPGraph *pg);
void             gts_pgraph_foreach_node     (GtsPGraph *pg,
                                              GtsFunc func,
                                              gpointer data);
gboolean         gts_pgraph_down             (GtsPGraph * pg,
                                              GtsFunc func,
                                              gpointer data);
/* Graph partition: partition.c */

GSList *         gts_graph_bubble_partition           (GtsGraph * g,
                                                       guint np,
                                                       guint niter,
                                                       GtsFunc step_info,
                                                       gpointer data);
guint            gts_graph_partition_edges_cut        (GSList * partition);
gfloat           gts_graph_partition_edges_cut_weight (GSList * partition);
void             gts_graph_partition_print_stats      (GSList * partition,
                                                       FILE * fp);
gfloat           gts_graph_partition_balance          (GSList * partition);
GSList *         gts_graph_partition_clone            (GSList * partition);
GSList *         gts_graph_recursive_bisection        (GtsWGraph * wg,
                                                       guint n,
                                                       guint ntry,
                                                       guint mmax,
                                                       guint nmin,
                                                       gfloat imbalance);
void             gts_graph_partition_destroy          (GSList * partition);

/* Graph bisection: partition.c */

typedef struct _GtsGraphBisection GtsGraphBisection;

struct _GtsGraphBisection {
  GtsGraph * g;
  GtsGraph * g1;
  GtsGraph * g2;
  GHashTable * bg1;
  GHashTable * bg2;
};

gboolean            gts_graph_bisection_check      (GtsGraphBisection * bg);
GtsGraphBisection * gts_graph_ggg_bisection        (GtsGraph * g,
                                                    guint ntry);
GtsGraphBisection * gts_graph_bfgg_bisection       (GtsGraph * g,
                                                    guint ntry);
gdouble             gts_graph_bisection_kl_refine  (GtsGraphBisection * bg,
                                                    guint mmax);
gdouble             gts_graph_bisection_bkl_refine (GtsGraphBisection * bg,
                                                    guint mmax,
                                                    gfloat imbalance);
GtsGraphBisection * gts_graph_bisection_new        (GtsWGraph * wg,
                                                    guint ntry,
                                                    guint mmax,
                                                    guint nmin,
                                                    gfloat imbalance);
void                gts_graph_bisection_destroy    (GtsGraphBisection * bg,
                                                    gboolean destroy_graphs);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTS_H__ */
