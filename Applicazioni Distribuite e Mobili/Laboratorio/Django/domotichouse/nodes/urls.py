from unicodedata import name
from django.urls import path
from . import views

app_name = 'nodes'

urlpatterns = [
    # Form-based interface: server-side only
    path('', views.node_form, name="node_form"),
    path('<int:node_id>/<slug:capability>', views.capability_form, name="capability_form"),
]
