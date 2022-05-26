from tkinter import N
from django.shortcuts import render, redirect
from django.http import HttpResponseRedirect
from django.core.mail import send_mail
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.db import transaction
from django.contrib.messages import success, error
from .forms import ContactForm, ProfileForm, UserForm

# Create your views here.

def contact(request):
    if request.method == 'POST':
        form = ContactForm(request.POST)
        if form.is_valid():
            subject = form.cleaned_data['subject']
            message = form.cleaned_data['message']
            sender = form.cleaned_data['sender']
            cc_myself = form.cleaned_data['cc_myself']

            recipients = ['info@example.com']
            if cc_myself:
                recipients.append(sender)
            send_mail(subject, message, sender, recipients)
            return HttpResponseRedirect('/thanks')
    else:
        form = ContactForm()
        return render(request, 'portal/contact.html', {'form': form})

@transaction.atomic
def register(request):
    if request.method == 'POST':
        user_form = UserForm(request.POST)
        profile_form = ProfileForm(request.POST)
        if user_form.is_valid() and profile_form.is_valid():
            user = user_form.save()
            profile_form.save()
            success(request, 'Success')
            return redirect('portal:welcome')
        else:
            error(request, 'Error')
            return render(request, 'registration/register.html', {'user_form': user_form, 'profile_form': profile_form})
    else:
        user_form = UserForm()
        profile_form = ProfileForm()
        return render(request, 'registration/register.html', {'user_form': user_form, 'profile_form': profile_form})


@login_required
def portal_welcome(request):
    """ 
        If users are authenticated, direct them to the portal welcome page
            (template index.html). Otherwise, take them to the login page.
    """
    return render(request, 'portal/index.html', {'request': request})

def login_view(request):
    username = request.POST['username']
    password = request.POST['password']
    user = authenticate(username=username, password=password)
    if user is not None:
        if user.is_active:
            login(request, user)
            # redirect to success page
        else:
            ...
            # return a 'disabled account' error message
    else:
        ...
        # return an 'invalid login' error message


def logout_view(request):
    logout(request)
    # redirect to success page