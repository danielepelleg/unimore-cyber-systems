from django.forms import ModelForm
from .models import * 

class NodeForm(ModelForm):
    class Meta:
        model = Node
        fields = ['id', 'name']

class TapparelleCapabilityForm(ModelForm):
    class Meta:
        model = TapparelleCapabilityEntry
        fields = ['id', 'name', 'description', 'node', 'pinUp', 'pinDown', 'timeout']

class TemperaturaCapabilityForm(ModelForm):
    class Meta:
        model = TemperaturaCapabilityEntry
        fields = ['id', 'name', 'description', 'node', 'pin', 'sensor', 'update']

class SwitchCapabilityForm(ModelForm):
    class Meta:
        model = SwitchCapabilityEntry
        fields = ['id', 'name', 'description', 'node', 'pin']

class ButtonCapabilityForm(ModelForm):
    class Meta:
        model = ButtonCapabilityEntry
        fields = ['id', 'name', 'description', 'node', 'pin', 'topic']