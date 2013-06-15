// representing tree as a set (without value)

#pragma once

#include "rbtree.h"
#include "strbuf.h"

void set_of_REG_to_string (rbtree *t, strbuf *out, unsigned limit);
void set_of_string_to_string (rbtree *t, strbuf *out, unsigned limit);
void set_add_string_or_free (rbtree *t, const char *s);

