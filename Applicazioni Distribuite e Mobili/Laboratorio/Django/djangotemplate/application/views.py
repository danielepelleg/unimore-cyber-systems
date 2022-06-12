from django.shortcuts import render
from django.views import generic

# Create your views here.

class ElementListView(generic.ListView):
    template_name = ... #'application/list.html'
    context_object_name = "elements_list"

    def get_queryset(self) -> list:
        return ...  # Element.objects.all(), Element.objects.filter(user = self.kwargs['user_id'])

class ElementDetailView(generic.DetailView):
    model = ...
    template_name = ... #'application/detail.html'
