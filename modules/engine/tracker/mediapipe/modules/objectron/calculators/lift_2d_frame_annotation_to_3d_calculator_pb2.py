# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: mediapipe/modules/objectron/calculators/lift_2d_frame_annotation_to_3d_calculator.proto

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
from mediapipe.modules.objectron.calculators import belief_decoder_config_pb2 as mediapipe_dot_modules_dot_objectron_dot_calculators_dot_belief__decoder__config__pb2


DESCRIPTOR = _descriptor.FileDescriptor(
  name='mediapipe/modules/objectron/calculators/lift_2d_frame_annotation_to_3d_calculator.proto',
  package='mediapipe',
  syntax='proto2',
  serialized_pb=_b('\nWmediapipe/modules/objectron/calculators/lift_2d_frame_annotation_to_3d_calculator.proto\x12\tmediapipe\x1a$mediapipe/framework/calculator.proto\x1a\x43mediapipe/modules/objectron/calculators/belief_decoder_config.proto\"\xda\x02\n*Lift2DFrameAnnotationTo3DCalculatorOptions\x12\x36\n\x0e\x64\x65\x63oder_config\x18\x01 \x01(\x0b\x32\x1e.mediapipe.BeliefDecoderConfig\x12\x1d\n\x12normalized_focal_x\x18\x02 \x01(\x02:\x01\x31\x12\x1d\n\x12normalized_focal_y\x18\x03 \x01(\x02:\x01\x31\x12\'\n\x1cnormalized_principal_point_x\x18\x04 \x01(\x02:\x01\x30\x12\'\n\x1cnormalized_principal_point_y\x18\x05 \x01(\x02:\x01\x30\x32\x64\n\x03\x65xt\x12\x1c.mediapipe.CalculatorOptions\x18\x8c\xac\xae\x8a\x01 \x01(\x0b\x32\x35.mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions')
  ,
  dependencies=[mediapipe_dot_framework_dot_calculator__pb2.DESCRIPTOR,mediapipe_dot_modules_dot_objectron_dot_calculators_dot_belief__decoder__config__pb2.DESCRIPTOR,])
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_LIFT2DFRAMEANNOTATIONTO3DCALCULATOROPTIONS = _descriptor.Descriptor(
  name='Lift2DFrameAnnotationTo3DCalculatorOptions',
  full_name='mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='decoder_config', full_name='mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions.decoder_config', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='normalized_focal_x', full_name='mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions.normalized_focal_x', index=1,
      number=2, type=2, cpp_type=6, label=1,
      has_default_value=True, default_value=float(1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='normalized_focal_y', full_name='mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions.normalized_focal_y', index=2,
      number=3, type=2, cpp_type=6, label=1,
      has_default_value=True, default_value=float(1),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='normalized_principal_point_x', full_name='mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions.normalized_principal_point_x', index=3,
      number=4, type=2, cpp_type=6, label=1,
      has_default_value=True, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='normalized_principal_point_y', full_name='mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions.normalized_principal_point_y', index=4,
      number=5, type=2, cpp_type=6, label=1,
      has_default_value=True, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
    _descriptor.FieldDescriptor(
      name='ext', full_name='mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions.ext', index=0,
      number=290166284, type=11, cpp_type=10, label=1,
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
  serialized_start=210,
  serialized_end=556,
)

_LIFT2DFRAMEANNOTATIONTO3DCALCULATOROPTIONS.fields_by_name['decoder_config'].message_type = mediapipe_dot_modules_dot_objectron_dot_calculators_dot_belief__decoder__config__pb2._BELIEFDECODERCONFIG
DESCRIPTOR.message_types_by_name['Lift2DFrameAnnotationTo3DCalculatorOptions'] = _LIFT2DFRAMEANNOTATIONTO3DCALCULATOROPTIONS

Lift2DFrameAnnotationTo3DCalculatorOptions = _reflection.GeneratedProtocolMessageType('Lift2DFrameAnnotationTo3DCalculatorOptions', (_message.Message,), dict(
  DESCRIPTOR = _LIFT2DFRAMEANNOTATIONTO3DCALCULATOROPTIONS,
  __module__ = 'mediapipe.modules.objectron.calculators.lift_2d_frame_annotation_to_3d_calculator_pb2'
  # @@protoc_insertion_point(class_scope:mediapipe.Lift2DFrameAnnotationTo3DCalculatorOptions)
  ))
_sym_db.RegisterMessage(Lift2DFrameAnnotationTo3DCalculatorOptions)

_LIFT2DFRAMEANNOTATIONTO3DCALCULATOROPTIONS.extensions_by_name['ext'].message_type = _LIFT2DFRAMEANNOTATIONTO3DCALCULATOROPTIONS
mediapipe_dot_framework_dot_calculator__options__pb2.CalculatorOptions.RegisterExtension(_LIFT2DFRAMEANNOTATIONTO3DCALCULATOROPTIONS.extensions_by_name['ext'])

# @@protoc_insertion_point(module_scope)
