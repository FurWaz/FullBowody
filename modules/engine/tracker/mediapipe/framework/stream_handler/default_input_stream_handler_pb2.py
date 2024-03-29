# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/framework/stream_handler/default_input_stream_handler.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from mediapipe.framework import mediapipe_options_pb2 as mediapipe_dot_framework_dot_mediapipe__options__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='mediapipe/framework/stream_handler/default_input_stream_handler.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\nEmediapipe/framework/stream_handler/default_input_stream_handler.proto\x12\tmediapipe\x1a+mediapipe/framework/mediapipe_options.proto\"\x93\x01\n DefaultInputStreamHandlerOptions\x12\x15\n\nbatch_size\x18\x01 \x01(\x05:\x01\x31\x32X\n\x03\x65xt\x12\x1b.mediapipe.MediaPipeOptions\x18\xf5\xed\xacN \x01(\x0b\x32+.mediapipe.DefaultInputStreamHandlerOptions')
  ,
  dependencies=[mediapipe_dot_framework_dot_mediapipe__options__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_DEFAULTINPUTSTREAMHANDLEROPTIONS = _descriptor.Descriptor(
  name='DefaultInputStreamHandlerOptions',
  full_name='mediapipe.DefaultInputStreamHandlerOptions',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='batch_size', full_name='mediapipe.DefaultInputStreamHandlerOptions.batch_size', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=True, default_value=1,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
    _descriptor.FieldDescriptor(
      name='ext', full_name='mediapipe.DefaultInputStreamHandlerOptions.ext', index=0,
      number=164312821, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=True, extension_scope=None,
      options=None),
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=130,
  serialized_end=277,
)

DESCRIPTOR.message_types_by_name['DefaultInputStreamHandlerOptions'] = _DEFAULTINPUTSTREAMHANDLEROPTIONS

DefaultInputStreamHandlerOptions = _reflection.GeneratedProtocolMessageType('DefaultInputStreamHandlerOptions', (_message.Message,), dict(
  DESCRIPTOR = _DEFAULTINPUTSTREAMHANDLEROPTIONS,
  __module__ = 'mediapipe.framework.stream_handler.default_input_stream_handler_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.DefaultInputStreamHandlerOptions)
  ))
_sym_db.RegisterMessage(DefaultInputStreamHandlerOptions)

_DEFAULTINPUTSTREAMHANDLEROPTIONS.extensions_by_name['ext'].message_type = _DEFAULTINPUTSTREAMHANDLEROPTIONS
mediapipe_dot_framework_dot_mediapipe__options__pb2.MediaPipeOptions.RegisterExtension(_DEFAULTINPUTSTREAMHANDLEROPTIONS.extensions_by_name['ext'])

# @@protoc_insertion_point(module_scope)
