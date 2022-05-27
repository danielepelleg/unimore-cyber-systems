from django.urls import path
from . import views

app_name = 'posts'

urlpatterns = [
    path('', views.index, name = 'index'),
    path('insertAuthor', views.insert_author, name = 'insert_author'),
    path('insertArticle', views.insert_article, name = 'insert_article'),
    path('<int:author>/posts/<int:year>/<int:month>/', views.PostsListView.as_view(), name = 'posts_list'),
    path('<int:author>/posts/<int:year>/<int:month>/<int:pk>', views.PostDetailView.as_view(), name = 'detail')
]
