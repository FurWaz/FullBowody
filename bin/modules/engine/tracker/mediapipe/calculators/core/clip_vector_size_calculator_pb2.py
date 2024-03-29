# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/calculators/core/clip_vector_size_calculator.proto

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
  name='mediapipe/calculators/core/clip_vector_size_calculator.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\n<mediapipe/calculators/core/clip_vector_size_calculator.proto\x12\tmediapipe\x1a$mediapipe/framework/calculator.proto\"\x95\x01\n\x1f\x43lipVectorSizeCalculatorOptions\x12\x17\n\x0cmax_vec_size\x18\x01 \x01(\x05:\x01\x31\x32Y\n\x03\x65xt\x12\x1c.mediapipe.CalculatorOptions\x18\xb6\xea\xfc\x82\x01 \x01(\x0b\x32*.mediapipe.ClipVectorSizeCalculatorOptionsB\x0c\xa2\x02\tMediaPipe')
  ,
  dependencies=[mediapipe_dot_framework_dot_calculator__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_CLIPVECTORSIZECALCULATOROPTIONS = _descriptor.Descriptor(
  name='ClipVectorSizeCalculatorOptions',
  full_name='mediapipe.ClipVectorSizeCalculatorOptions',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='max_vec_size', full_name='mediapipe.ClipVectorSizeCalculatorOptions.max_vec_size', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=True, default_value=1,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
    _descriptor.FieldDescriptor(
      name='ext', full_name='mediapipe.ClipVectorSizeCalculatorOptions.ext', index=0,
      number=274674998, type=11, cpp_type=10, label=1,
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
  serialized_start=114,
  serialized_end=263,
)

DESCRIPTOR.message_types_by_name['ClipVectorSizeCalculatorOptions'] = _CLIPVECTORSIZECALCULATOROPTIONS

ClipVectorSizeCalculatorOptions = _reflection.GeneratedProtocolMessageType('ClipVectorSizeCalculatorOptions', (_message.Message,), dict(
  DESCRIPTOR = _CLIPVECTORSIZECALCULATOROPTIONS,
  __module__ = 'mediapipe.calculators.core.clip_vector_size_calculator_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.ClipVectorSizeCalculatorOptions)
  ))
_sym_db.RegisterMessage(ClipVectorSizeCalculatorOptions)

_CLIPVECTORSIZECALCULATOROPTIONS.extensions_by_name['ext'].message_type = _CLIPVECTORSIZECALCULATOROPTIONS
mediapipe_dot_framework_dot_calculator__options__pb2.CalculatorOptions.RegisterExtension(_CLIPVECTORSIZECALCULATOROPTIONS.extensions_by_name['ext'])

DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), _b('\242\002\tMediaPipe'))
# @@protoc_insertion_point(module_scope)
