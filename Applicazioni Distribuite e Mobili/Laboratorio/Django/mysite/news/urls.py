from django.urls import path, register_converter
from . import views, converters

app_name = 'articles'

register_converter(converters.FourDigitYearConverter, 'yyyy')
register_converter(converters.TwoDigitMonthConveter, 'mm')

urlpatterns = [
    path('', views.index, name='index'),
    path('<yyyy:year>', views.year_archive),
    path('<yyyy:year>/<mm:month>', views.month_archive)
]