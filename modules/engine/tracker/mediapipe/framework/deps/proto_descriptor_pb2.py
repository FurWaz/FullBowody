# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/framework/deps/proto_descriptor.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='mediapipe/framework/deps/proto_descriptor.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\n/mediapipe/framework/deps/proto_descriptor.proto\x12\tmediapipe\"\xe1\x02\n\x14\x46ieldDescriptorProto\"\xc8\x02\n\x04Type\x12\x10\n\x0cTYPE_INVALID\x10\x00\x12\x0f\n\x0bTYPE_DOUBLE\x10\x01\x12\x0e\n\nTYPE_FLOAT\x10\x02\x12\x0e\n\nTYPE_INT64\x10\x03\x12\x0f\n\x0bTYPE_UINT64\x10\x04\x12\x0e\n\nTYPE_INT32\x10\x05\x12\x10\n\x0cTYPE_FIXED64\x10\x06\x12\x10\n\x0cTYPE_FIXED32\x10\x07\x12\r\n\tTYPE_BOOL\x10\x08\x12\x0f\n\x0bTYPE_STRING\x10\t\x12\x0e\n\nTYPE_GROUP\x10\n\x12\x10\n\x0cTYPE_MESSAGE\x10\x0b\x12\x0e\n\nTYPE_BYTES\x10\x0c\x12\x0f\n\x0bTYPE_UINT32\x10\r\x12\r\n\tTYPE_ENUM\x10\x0e\x12\x11\n\rTYPE_SFIXED32\x10\x0f\x12\x11\n\rTYPE_SFIXED64\x10\x10\x12\x0f\n\x0bTYPE_SINT32\x10\x11\x12\x0f\n\x0bTYPE_SINT64\x10\x12')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)



_FIELDDESCRIPTORPROTO_TYPE = _descriptor.EnumDescriptor(
  name='Type',
  full_name='mediapipe.FieldDescriptorProto.Type',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='TYPE_INVALID', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_DOUBLE', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_FLOAT', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_INT64', index=3, number=3,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_UINT64', index=4, number=4,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_INT32', index=5, number=5,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_FIXED64', index=6, number=6,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_FIXED32', index=7, number=7,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_BOOL', index=8, number=8,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_STRING', index=9, number=9,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_GROUP', index=10, number=10,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_MESSAGE', index=11, number=11,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_BYTES', index=12, number=12,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_UINT32', index=13, number=13,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_ENUM', index=14, number=14,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_SFIXED32', index=15, number=15,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_SFIXED64', index=16, number=16,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_SINT32', index=17, number=17,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='TYPE_SINT64', index=18, number=18,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=88,
  serialized_end=416,
)
_sym_db.RegisterEnumDescriptor(_FIELDDESCRIPTORPROTO_TYPE)


_FIELDDESCRIPTORPROTO = _descriptor.Descriptor(
  name='FieldDescriptorProto',
  full_name='mediapipe.FieldDescriptorProto',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _FIELDDESCRIPTORPROTO_TYPE,
  ],
  options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=63,
  serialized_end=416,
)

_FIELDDESCRIPTORPROTO_TYPE.containing_type = _FIELDDESCRIPTORPROTO
DESCRIPTOR.message_types_by_name['FieldDescriptorProto'] = _FIELDDESCRIPTORPROTO

FieldDescriptorProto = _reflection.GeneratedProtocolMessageType('FieldDescriptorProto', (_message.Message,), dict(
  DESCRIPTOR = _FIELDDESCRIPTORPROTO,
  __module__ = 'mediapipe.framework.deps.proto_descriptor_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.FieldDescriptorProto)
  ))
_sym_db.RegisterMessage(FieldDescriptorProto)


# @@protoc_insertion_point(module_scope)
