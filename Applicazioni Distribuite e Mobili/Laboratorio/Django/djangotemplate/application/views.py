from django.http import HttpResponse, JsonResponse
from rest_framework.parsers import JSONParser
from django.shortcuts import render
from django.views import generic
from django.utils import timezone
from django.contrib.auth.decorators import login_required
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_POST
from .models import *
from .forms import *
from .serializers import *


# Create your views here.

class ElementListView(generic.ListView):
    template_name = ... #'application/list.html'
    context_object_name = "elements_list"

    def get_queryset(self) -> list:
        return ...  # Element.objects.all(), Element.objects.filter(user = self.kwargs['user_id'])

    # Needed if URL Parameters are requested
    def get_context_data(self):
        context = super().get_context_data(**self.kwargs)
        return context

class ElementDetailView(generic.DetailView):
    model = ...
    context_object_name = 'element'
    template_name = ... #'application/detail.html'

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**self.kwargs)
        return context


def jquery_list(request):
    url = f"/application/"
    return render(request, 'application/jquery.html', {'url': url, 'script_name': 'list.js'})
