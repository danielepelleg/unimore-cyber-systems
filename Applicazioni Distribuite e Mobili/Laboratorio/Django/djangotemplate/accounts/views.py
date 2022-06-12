from django.http import HttpResponse, JsonResponse
from django.shortcuts import render
from django.contrib.auth.forms import UserCreationForm
from django.views.decorators.csrf import csrf_exempt
from .forms import ProfileForm
from .models import User, Profile
from .serializers import UserSerializer, ProfileSerializer

# Create your views here.

def register(request):
    if request.method == 'POST':
        user_form = UserCreationForm(request.POST)
        profile_form = ProfileForm(request.POST)
        if user_form.is_valid():
            user = user_form.save()
        if profile_form.is_valid():
            profile = profile_form.save(commit=False)
            profile.user = user
            profile.save()
            return HttpResponse("Registration Success!")
    else:
        user_form = UserCreationForm()
        profile_form = ProfileForm()
        return render(request, 'registration/register.html', {'user_form': user_form, 'profile_form': profile_form})

def test(request):
    return render(request, 'elements/empty.html')

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
        users = Profile.objects.all()
        serializer = ProfileSerializer(users, many=True)
        return JsonResponse(serializer.data, safe=False)