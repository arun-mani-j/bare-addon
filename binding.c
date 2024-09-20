#include <assert.h>
#include <bare.h>
#include <js.h>
#include <stdio.h>
#include <utf.h>

static js_value_t *
bare_addon_hello (js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[argc];
  err = js_get_callback_info (env, info, &argc, argv, NULL, NULL);
  assert (err == 0);

  size_t len = 1024;
  size_t size;
  utf8_t filename[len];
  err = js_get_value_string_utf8 (env, argv[0], filename, len - 1, &size);
  assert (err == 0);

  FILE *fp = fopen ((const char *) filename, "r");
  if (fp == NULL) {
    js_throw_error (env, "C", "File not found");
    return NULL;
  }

  char buf[len];
  if (fgets (buf, len, fp) == NULL) {
    js_throw_error (env, "C", "File is empty");
    return NULL;
  }

  js_value_t *result;
  err = js_create_string_utf8 (env, (utf8_t *) buf, -1, &result);
  if (err < 0) return NULL;

  return result;
}

static js_value_t *
bare_addon_exports (js_env_t *env, js_value_t *exports) {
  int err;

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, NULL, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("hello", bare_addon_hello)
#undef V

  return exports;
}

BARE_MODULE(bare_addon, bare_addon_exports)
