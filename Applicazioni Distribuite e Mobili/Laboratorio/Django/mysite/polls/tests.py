from django.test import TestCase
from django.utils import timezone
from django.urls import reverse
from .models import Question
import datetime

# Create your tests here.

class QuestionMethodTests(TestCase):
    def test_was_published_recently_with_future_question(self):
        """
            was_published_recently() should return False for questions whose 
                pub_date is in the future 
        """
        time = timezone.now() + datetime.timedelta(days=30)
        future_question = Question("Question?", pub_date=time)
        self.assertEqual(future_question.was_published_recently(), False)

    def test_was_published_recently_with_old_question(self):
        """
            was_published_recently() should return False for questions whose 
                pub_date is older than 1 day
        """
        time = timezone.now() - datetime.timedelta(days=30)
        old_question = Question("Old?", pub_date=time)
        self.assertEqual(old_question.was_published_recently(), False)
    
    def test_was_published_recently_with_recent_question(self):
        """
            was_published_recently() should return True for questions whose 
                pub_date is within the last day
        """
        time = timezone.now() - datetime.timedelta(hours=1)
        recent_question = Question("Recent?", pub_date=time)
        self.assertEqual(recent_question.was_published_recently(), True)

def create_question(question_text, days):
    """
        Creates a question: days represernt the offset to now: negative
            for questions published in the past, positive for those in the future
    """
    time = timezone.now() + datetime.timedelta(days=days)
    return Question.objects.create(question_text=question_text, pub_date=time)

class QuestionViewCase(TestCase):
    def test_index_view_with_no_questions(self):
        """
            No questions --> "No polls are available" message desplayed
        """
        response = self.client.get(reverse('polls:index'))
        self.assertEqual(response.status_code, 200)
        self.assertContains(response, "No polls are available")
        self.assertQuerysetEqual(response.context['latest_question_list'], [])

    def test_index_view_with_a_past_question(self):
        """
            Questions with a pub date in the past should be displayed
        """
        create_question(question="Past question.", days=-30)
        response = self.client.get(reverse('polls:index'))
        self.assertEqual(response.context['latest_question_list'], ['<Question: Past question.>'])
    
    def test_index_view_with_a_future_question(self):
        """
            Questions with a pub_date in the future --> not shown
        """
        create_question(question_text="Future question.", days=30)
        response = self.client.get(reverse('polls:index'))
        self.assertEqual(response.status_code, 200)
        self.assertContains(response, "No polls are available")
        self.assertQuerysetEqual(response.context['latest_question_list'], [])

    def test_index_view_with_future_question_and_past_question(self):
        """ 
            Even if both past and future polls exist, only pasrt polls should be displayed.
        """
        create_question(question="Past question.", days=-30)
        create_question(question_text="Future question.", days=30)
        response = self.client.get(reverse('polls:index'))
        self.assertEqual(response.context['latest_question_list'], ['<Question: Past question.>'])