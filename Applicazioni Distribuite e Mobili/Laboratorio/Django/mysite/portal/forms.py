from email import message
from django import forms
from django.contrib.auth.models import User
from .models import Profile, Skill

class ContactForm(forms.Form):
    subject = forms.CharField(max_length=100)
    message = forms.CharField() 
    sender = forms.EmailField()
    cc_myself = forms.BooleanField(required=False)

class UserForm(forms.ModelForm):
    class Meta:
        model = User
        fields = ['first_name', 'last_name', 'username', 'password']

class ProfileForm(forms.ModelForm):
    class Meta:
        model = Profile
        fields = ['birth_date', 'image', 'country', 'city']

    # image = forms.ImageField()
    # country = forms.CharField()
    # city = forms.CharField()
    # skill = forms.MultipleChoiceField(queryset=Skill.objects.all(), widget=forms.CheckboxSelectMultiple)

class PersonForm(forms.Form):
    first_name = forms.CharField()
    last_name = forms.CharField()