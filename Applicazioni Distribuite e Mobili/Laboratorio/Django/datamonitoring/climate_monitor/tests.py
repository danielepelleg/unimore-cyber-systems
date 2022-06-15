from django.forms import ValidationError
from django.test import TestCase
from django.utils import timezone
from .models import Sensor, Detection
import datetime 

# Create your tests here.

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