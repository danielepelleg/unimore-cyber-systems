from django.urls import path, include
from . import views

app_name = 'climate_monitor'

urlpatterns = [
    path('<str:type>/', views.SensorListView.as_view(), name='sensor_list'),
]
