from dataclasses import field
from django.forms import ModelForm
from .models import Author, Book

class AuthorForm(ModelForm):
    class Meta:
        model = Author
        fields = ['name', 'title', 'nationality']

class BookForm(ModelForm):
    class Meta:
        model = Book
        fields = ['name', 'authors']