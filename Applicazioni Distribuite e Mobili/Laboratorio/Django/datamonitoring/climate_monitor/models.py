from django.db import models
from django.utils import timezone

# Create your models here.

class Sensor(models.Model):
    SENSOR_TYPE = [('TEMPERATURA', 'TEMPERATURA'), ('PM10', 'PM10')]
    type = models.CharField(default='TEMPERATURA', choices=SENSOR_TYPE, max_length=11)
    address = models.CharField(max_length=50)
    cap = models.DecimalField(max_digits=5, decimal_places=0)

    def get_fields_values(self):
        return [(field.attname, field.value_to_string(self)) for field in Sensor._meta.fields]
    
    @property
    def average_value(self):
        average = 0
        detections = Detection.objects.filter(sensor=self)
        for detect in detections:
            average += detect.value
        if average != 0:
            return "{:.2f}".format(average / len(detections))
        return average
        
    
    def __str__(self) -> str:
        return str('S.' + str(self.id) + ' ' + self.type)

class Detection(models.Model):
    SENSOR_TYPE = [('TEMPERATURA', 'TEMPERATURA'), ('PM10', 'PM10')]
    sensor = models.ForeignKey(Sensor, on_delete=models.CASCADE)
    type = models.CharField(default='TEMPERATURA', choices=SENSOR_TYPE, max_length=11)
    timestamp = models.DateTimeField(default=timezone.now)
    value = models.IntegerField()

    def clean(self):
        from django.core.exceptions import ValidationError
        if self.timestamp > timezone.now():
            raise ValidationError({'timestamp': ('Enter a timestamp < now.')})
        if self.type != self.sensor.type:
            raise ValidationError({'type': ('Detection type and sensor type mismatch.')})
        super().clean()

    def get_fields_values(self):
        return [(field.attname, field.value_to_string(self)) for field in Detection._meta.fields]

    def __str__(self) -> str:
        return str( 'D.' + str(self.id) + ' Sensor: (' + str(self.sensor) + ') ' + self.type + ' ' + str(self.timestamp)) 