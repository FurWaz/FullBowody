# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/calculators/util/association_calculator.proto

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
  name='mediapipe/calculators/util/association_calculator.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\n7mediapipe/calculators/util/association_calculator.proto\x12\tmediapipe\x1a$mediapipe/framework/calculator.proto\"\x9b\x01\n\x1c\x41ssociationCalculatorOptions\x12#\n\x18min_similarity_threshold\x18\x01 \x01(\x02:\x01\x31\x32V\n\x03\x65xt\x12\x1c.mediapipe.CalculatorOptions\x18\xef\xa4\x98\x83\x01 \x01(\x0b\x32\'.mediapipe.AssociationCalculatorOptions')
  ,
  dependencies=[mediapipe_dot_framework_dot_calculator__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_ASSOCIATIONCALCULATOROPTIONS = _descriptor.Descriptor(
  name='AssociationCalculatorOptions',
  full_name='mediapipe.AssociationCalculatorOptions',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='min_similarity_threshold', full_name='mediapipe.AssociationCalculatorOptions.min_similarity_threshold', index=0,
      number=1, type=2, cpp_type=6, label=1,
      has_default_value=True, default_value=float(1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
    _descriptor.FieldDescriptor(
      name='ext', full_name='mediapipe.AssociationCalculatorOptions.ext', index=0,
      number=275124847, type=11, cpp_type=10, label=1,
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
  serialized_start=109,
  serialized_end=264,
)

DESCRIPTOR.message_types_by_name['AssociationCalculatorOptions'] = _ASSOCIATIONCALCULATOROPTIONS

AssociationCalculatorOptions = _reflection.GeneratedProtocolMessageType('AssociationCalculatorOptions', (_message.Message,), dict(
  DESCRIPTOR = _ASSOCIATIONCALCULATOROPTIONS,
  __module__ = 'mediapipe.calculators.util.association_calculator_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.AssociationCalculatorOptions)
  ))
_sym_db.RegisterMessage(AssociationCalculatorOptions)

_ASSOCIATIONCALCULATOROPTIONS.extensions_by_name['ext'].message_type = _ASSOCIATIONCALCULATOROPTIONS
mediapipe_dot_framework_dot_calculator__options__pb2.CalculatorOptions.RegisterExtension(_ASSOCIATIONCALCULATOROPTIONS.extensions_by_name['ext'])

# @@protoc_insertion_point(module_scope)
