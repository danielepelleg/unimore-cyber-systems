from django.forms import ModelForm, DateInput
from .models import Profile

class DateInput(DateInput):
    input_type = 'date'

class ProfileForm(ModelForm):
    class Meta:
        model = Profile
        fields = ['gender', 'location', 'birth_date']
        labels = {
            'birth_date': 'Birth date'
        }
        widget = {
            'birth_date': DateInput()
        }