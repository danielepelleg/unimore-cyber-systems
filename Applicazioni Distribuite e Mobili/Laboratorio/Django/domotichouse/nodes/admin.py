from django.contrib import admin
from . import models

# Register your models here.
admin.site.register(models.Node)
admin.site.register(models.TapparelleCapabilityEntry)
admin.site.register(models.TemperaturaCapabilityEntry)