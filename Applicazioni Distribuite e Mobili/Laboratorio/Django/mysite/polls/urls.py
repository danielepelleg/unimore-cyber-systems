from cgi import test
from django.urls import path, re_path
from . import views

app_name = 'polls'

urlpatterns = [
    path('', views.IndexView.as_view(), name='index'),
    path('count/', views.count, name='count'),
    path('<int:pk>/', views.DetailView.as_view(), name='detail'),
    path('<int:pk>/results', views.ResultsView.as_view(), name='results'),
    path('<int:question_id>/vote', views.vote, name='vote'),
    path('test_email/', views.email_test, name='test_email'),
    path('test_vote/', views.vote_test, name='test_vote')
]