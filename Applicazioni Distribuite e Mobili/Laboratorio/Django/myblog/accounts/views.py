from django.http import HttpResponse, JsonResponse
from django.shortcuts import render
from django.contrib.auth.forms import UserCreationForm
from django.views.decorators.csrf import csrf_exempt
from .models import User
from posts.forms import AuthorForm
from posts.models import Author
from posts.serializers import AuthorDetailSerializer
from .serializers import UserSerializer


# Create your views here.

def register(request):
    if request.method == 'POST':
        user_form = UserCreationForm(request.POST)
        profile_form = AuthorForm(request.POST)
        if user_form.is_valid():
            user = user_form.save()
        if profile_form.is_valid():
            author = profile_form.save(commit=False)
            author.user = user
            author.save()
            return HttpResponse("Registration Success!")
    else:
        user_form = UserCreationForm()
        profile_form = AuthorForm()
        return render(request, 'registration/register.html', {'user_form': user_form, 'profile_form': profile_form})

"""
    REST API
"""
@csrf_exempt
def users_list_api(request):
    if request.method == 'GET':
        users = User.objects.all()
        serializer = UserSerializer(users, many=True)
        return JsonResponse(serializer.data, safe=False)

@csrf_exempt
def profiles_list_api(request):
    if request.method == 'GET':
        users = Author.objects.all()
        serializer = AuthorDetailSerializer(users, many=True)
        return JsonResponse(serializer.data, safe=False)