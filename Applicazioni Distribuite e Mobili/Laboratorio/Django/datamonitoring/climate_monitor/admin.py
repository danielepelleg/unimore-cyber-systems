from django.contrib import admin
from .models import Sensor, Detection

# Register your models here.

class DetectionInlineForm(admin.StackedInline):
    model = Detection
    extra = 2

class SensorDetection(admin.ModelAdmin):
    model = Sensor
    inlines = [DetectionInlineForm]

admin.site.register(Sensor, SensorDetection)
admin.site.register(Detection)
