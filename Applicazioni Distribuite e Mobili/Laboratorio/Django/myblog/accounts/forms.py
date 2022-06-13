from django.forms import ModelForm, DateInput

class DateInput(DateInput):
    input_type = 'date'