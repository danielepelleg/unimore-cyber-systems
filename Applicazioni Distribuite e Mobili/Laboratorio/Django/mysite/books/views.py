from django.http import HttpResponse
from django.shortcuts import render
from .forms import AuthorForm, BookForm
from .models import Author

# Create your views here.

def insertAuth(request):
    if request.method == 'GET':
        form = AuthorForm()
        return render(request, 'books/insertForm.html', {'form': form})
    else:
        form = AuthorForm(request.POST)
        form.save()
        return HttpResponse('New Author has been saved')

def modifyAuth(request, author_id):
    if request.method == 'GET':
        author_instance = Author.objects.get(pk=author_id)
        form = AuthorForm(instance=author_instance)
        return render(request, 'books/insertForm.html', {'form': form})
    else:
        author_instance = Author.objects.get(pk=author_id)
        form = AuthorForm(request.POST, instance=author_instance)
        form.save()
        return HttpResponse(f'Author {author_id} has been modified')