from django.shortcuts import render
from django.views import generic
from .models import Sensor

# Create your views here.

class SensorListView(generic.ListView):
    template_name = 'climate_monitor/list.html'
    context_object_name = "elements_list"

    def get_queryset(self) -> list:
        type = str(self.kwargs['type']).upper()
        print(type)
        return Sensor.objects.filter(type=type)

    # Needed if URL Parameters are requested
    def get_context_data(self):
        context = super().get_context_data(**self.kwargs)
        return context

def jquery_list(request):
    url = f"/application/"
    return render(request, 'application/jquery.html', {'url': url, 'script_name': 'list.js'})
