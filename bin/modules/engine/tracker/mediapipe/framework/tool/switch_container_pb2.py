# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/framework/tool/switch_container.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from mediapipe.framework import calculator_pb2 as mediapipe_dot_framework_dot_calculator__pb2
mediapipe_dot_framework_dot_calculator__options__pb2 = mediapipe_dot_framework_dot_calculator__pb2.mediapipe_dot_framework_dot_calculator__options__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='mediapipe/framework/tool/switch_container.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\n/mediapipe/framework/tool/switch_container.proto\x12\tmediapipe\x1a$mediapipe/framework/calculator.proto\"\xcf\x01\n\x16SwitchContainerOptions\x12=\n\x0e\x63ontained_node\x18\x02 \x03(\x0b\x32%.mediapipe.CalculatorGraphConfig.Node\x12\x0e\n\x06select\x18\x03 \x01(\x05\x12\x0e\n\x06\x65nable\x18\x04 \x01(\x08\x32P\n\x03\x65xt\x12\x1c.mediapipe.CalculatorOptions\x18\xe2\x9a\xfc\xa4\x01 \x01(\x0b\x32!.mediapipe.SwitchContainerOptionsJ\x04\x08\x01\x10\x02\x42\x32\n\x1a\x63om.google.mediapipe.protoB\x14SwitchContainerProto')
  ,
  dependencies=[mediapipe_dot_framework_dot_calculator__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_SWITCHCONTAINEROPTIONS = _descriptor.Descriptor(
  name='SwitchContainerOptions',
  full_name='mediapipe.SwitchContainerOptions',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='contained_node', full_name='mediapipe.SwitchContainerOptions.contained_node', index=0,
      number=2, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='select', full_name='mediapipe.SwitchContainerOptions.select', index=1,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='enable', full_name='mediapipe.SwitchContainerOptions.enable', index=2,
      number=4, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
    _descriptor.FieldDescriptor(
      name='ext', full_name='mediapipe.SwitchContainerOptions.ext', index=0,
      number=345967970, type=11, cpp_type=10, label=1,
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
  serialized_start=101,
  serialized_end=308,
)

_SWITCHCONTAINEROPTIONS.fields_by_name['contained_node'].message_type = mediapipe_dot_framework_dot_calculator__pb2._CALCULATORGRAPHCONFIG_NODE
DESCRIPTOR.message_types_by_name['SwitchContainerOptions'] = _SWITCHCONTAINEROPTIONS

SwitchContainerOptions = _reflection.GeneratedProtocolMessageType('SwitchContainerOptions', (_message.Message,), dict(
  DESCRIPTOR = _SWITCHCONTAINEROPTIONS,
  __module__ = 'mediapipe.framework.tool.switch_container_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.SwitchContainerOptions)
  ))
_sym_db.RegisterMessage(SwitchContainerOptions)

_SWITCHCONTAINEROPTIONS.extensions_by_name['ext'].message_type = _SWITCHCONTAINEROPTIONS
mediapipe_dot_framework_dot_calculator__options__pb2.CalculatorOptions.RegisterExtension(_SWITCHCONTAINEROPTIONS.extensions_by_name['ext'])

DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), _b('\n\032com.google.mediapipe.protoB\024SwitchContainerProto'))
# @@protoc_insertion_point(module_scope)
