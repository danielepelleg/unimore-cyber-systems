from socket import timeout
from tkinter import Button
from django.db import models

# Create your models here.


class Node(models.Model):
    name = models.CharField(max_length=40)

    def __str__(self) -> str:
        return self.name


class GenericCapabilityEntry(models.Model):
    name = models.CharField(max_length=40)
    node = models.ForeignKey(Node, on_delete=models.CASCADE)
    capability = "generic"

    def get_topic(self):
        return "home/" + self.node.name + "/" + self.capability + "/" + self.name


class TapparelleCapabilityEntry(GenericCapabilityEntry):
    capability = "tapparelle"
    description = models.CharField(max_length=255)
    timeout = models.IntegerField()
    pinUp = models.CharField(max_length=10)
    pinDown = models.CharField(max_length=10)


class TemperaturaCapabilityEntry(GenericCapabilityEntry):
    capability = "tapparelle"
    description = models.CharField(max_length=255)
    update = models.IntegerField()
    pin = models.CharField(max_length=10)
    sensor = models.CharField(max_length=10)


class SwitchCapabilityEntry(GenericCapabilityEntry):
    capability = "switch"
    description = models.CharField(max_length=255)
    # Longer pin field because we can have multiple pins
    # for a single switch
    pin = models.CharField(max_length=255)

    def __str__(self) -> str:
        return self.description


class ButtonCapabilityEntry(GenericCapabilityEntry):
    capability = "button"
    description = models.CharField(max_length=200)
    pin = models.CharField(max_length=10)
    # Multiple topic can be supported
    topic = models.ForeignKey(SwitchCapabilityEntry, on_delete=models.CASCADE)


capabilities = {
    "tapparelle": {
        "name": "tapparelle",
        "model": "TapparelleCapabilityEntry",
        "form": "TapparelleCapabilityForm",
        "serializer": "TapparelleCapabilitySerializer",
    },
    "temperatura": {
        "name": "temperatura",
        "model": "TemperaturaCapabilityEntry",
        "form": "TemperaturaCapabilityForm",
        "serializer": "TemperaturaCapabilitySerializer",
    },
    "switch": {
        "name": "switch",
        "model": "SwitchCapabilityEntry",
        "form": "SwitchCapabilityForm",
        "serializer": "SwitchCapabilitySerializer",
    },
    "button": {
        "name": "button",
        "model": "ButtonCapabilityEntry",
        "form": "ButtonCapabilityForm",
        "serializer": "ButtonCapabilitySerializer",
    }
}
