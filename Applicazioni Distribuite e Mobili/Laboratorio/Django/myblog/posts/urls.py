from django.urls import path
from . import views

app_name = 'posts'

urlpatterns = [
    path('', views.index, name = 'index'),
    path('insertAuthor', views.insert_author, name = 'insert_author'),
    path('insertArticle', views.insert_article, name = 'insert_article'),
    path('posts/<int:element_id>/edit', views.edit_post, name='edit_post'),
    path('posts/search', views.search, name='search'),
    path('<int:user_id>/posts/', views.UserPostsListView.as_view(), name = 'user_posts_list'),
    path('<int:user_id>/posts/<int:year>/<int:month>/', views.PostsYearMonthListView.as_view(), name = 'posts_list_year_month'),
    path('<int:user_id>/posts/<int:year>/<int:month>/jquery', views.jquery_list, name = 'jquery_list'),
    path('<int:user_id>/posts/<int:year>/<int:month>/<int:pk>/', views.PostDetailView.as_view(), name = 'detail'),
    path('<int:user_id>/posts/<int:year>/<int:month>/<int:post_id>/jquery', views.jquery_detail, name = 'jquery_detail'),
    # API
    path('api/posts/<int:post_id>', views.detail_post_api, name='detail_post_api'),
    path('api/<int:user_id>/posts/<int:year>/<int:month>', views.list_year_month_posts_api, name="list_year_month_posts_api"),
    path('api/<int:user_id>/posts/<int:year>/<int:month>/<int:post_id>', views.detail_year_month_posts_api, name="detail_year_month_posts_api"),
]
