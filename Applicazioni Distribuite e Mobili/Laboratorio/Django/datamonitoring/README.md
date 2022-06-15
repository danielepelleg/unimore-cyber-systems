<br />    
<p align="center">
<h1 align="center">Django Exam - Data Monitoring</h1>
<p align="center">
    Django Exam 12.01.2022
</p>

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Getting Started](#getting-started)
- [Exam](#exam)
  - [Models](#models)
  - [Urls](#urls)
  - [Tests](#tests)

## Getting Started

1. Create Python Enviroment by running:

```bash
$> python -m venv env
```

Use `python3` instead of `python` command if you use a UNIX system.

Create then a `.env` where the following keys must be specified:

- `DATABASE_NAME`
- `DATABASE_USER`
- `DATABASE_PASSWORD`
- `SECRET_KEY`=datamonitoring
- `PROJECT_NAME`

1. Install the python dependencies by running:

```bash
$> pip install -r requirements.txt
```

4. Create Superuser and populate the database:

```bash
$> python manage.py createsuperuser
```

Once the superuser has been created, run the server:

```bash
$> python manage.py runserver
```

Navigate to: [localhost/admin](http://127.0.0.1:8000/admin) and login to start populate the database. Create some Sensors and Detections to begin.

## Exam

### Models
```python
# ./climate_monitor/models.py

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
```

### Urls
```python
# ./climate_monitor/urls.py

urlpatterns = [
    path('<str:type>/', views.SensorListView.as_view(), name='sensor_list'),
]

# ./climate_monitor/views.py

class SensorListView(generic.ListView):
    template_name = 'climate_monitor/list.html'
    context_object_name = "elements_list"

    def get_queryset(self) -> list:
        type = str(self.kwargs['type']).upper()
        print(type)
        return Sensor.objects.filter(type=type)

    # Needed if URL Parameters are requested
    def get_context_data(self):
        context = super().get_context_data(**self.kwargs)
        return context
```

### Tests
```python
# ./climate_monitor/tests.py

class DetectionIsValidMethodTests(TestCase):
    def test_is_valid_with_future_timestamp(self):
        """
            clean() should raise ValidationError for timestamp 
                whose value is in the future
        """
        future_time = timezone.now() + datetime.timedelta(days=30)
        test_sensor = Sensor("TEMPERATURA", "Ingegneria Modens", 43100)
        new_detection = Detection(sensor=test_sensor, type="TEMPERATURA", timestamp=future_time, value=1)
        self.assertRaises(ValidationError, new_detection.clean)
    
    def test_is_valid_with_different_type(self):
        """
            clean() should raise ValidationError for type field 
                whose value is different from the one of its related sensor
        """
        test_sensor = Sensor("TEMPERATURA", "Ingegneria Modena", 43100)
        new_detection = Detection(sensor=test_sensor, type="PM10", value=1)
        self.assertRaises(ValidationError, new_detection.clean)
```
To run test open the bash and execute the command:
```bash
$>  python manage.py test climate_monitor
```