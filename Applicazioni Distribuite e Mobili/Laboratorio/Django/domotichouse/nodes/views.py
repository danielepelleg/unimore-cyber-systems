from django.shortcuts import render
from django.http import Http404
from django.forms import modelformset_factory
import importlib
from . import models
from . import forms

# Create your views here.

def node_form(request):
    NodeFormset = modelformset_factory(models.Node, form=forms.NodeForm, can_delete=True)
    if request.method == 'POST':
        formset = NodeFormset(request.POST)
        if formset.is_valid():
            formset.save()
    else:
        formset = NodeFormset()
    return render(request, 'nodes/nodes.html', {'formset': formset, "capabilities": models.capabilities})

def capability_form(request, node_id, capability):
    # get model class
    if capability in models.capabilities:
        cap = models.capabilities[capability]
        model = getattr(importlib.import_module("nodes.models"), cap["model"])
        form = getattr(importlib.import_module("nodes.forms"), cap["form"])
    else:
        raise Http404("Capability Not Found")
    CapabilityFormset = modelformset_factory(model, form=form, can_delete=True)
    
    if request.method == 'POST':
        formset = CapabilityFormset(request.POST, queryset=model.objects.filter(node__exact = node_id))
        if formset.is_valid():
            formset.save()
    else:
        formset = CapabilityFormset(queryset = model.objects.filter(node__exact = node_id))
        return render(request, "nodes/capabilities.html", {'formset': formset})