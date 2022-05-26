from django.http import HttpResponse, HttpResponseRedirect
from django.urls import reverse
from django.shortcuts import get_object_or_404, render
from django.views import generic
from django.contrib.auth.decorators import login_required, user_passes_test, permission_required
from django.utils import timezone
from .models import Choice, Question


# Create your views here.

class IndexView(generic.ListView):
    template_name = "polls/index.html"
    context_object_name = 'latest_question_list' # default: question_list

    # definisce il valore della lista da passare al template
    def get_queryset(self):
        """ 
            Return the last five published polls not including
                those set top be published in the future
        """
        return Question.objects.filter(pub_date__lte=timezone.now()).order_by('-pub_date')[:5]
    

def index(request): 
    latest_question_list = Question.objects.order_by('-pub_date')[:5]
    context = {'latest_question_list': latest_question_list}
    return render(request, template_name='polls/index.html', context=context)

class DetailView(generic.DetailView):
    model = Question
    template_name = "polls/detail.html"

def detail(request, question_id):
    # try:
    #     question = Question.objects.get(pk=question_id)
    # except Question.DoesNotExist:
    #     raise Http404("Question does not exist")
    question = get_object_or_404(Question, pk=question_id )
    context = {'question': question}
    return render(request, 'polls/detail.html', context)

class ResultsView(generic.DetailView):
    model = Question
    template_name = 'polls/results.html'

def results(request, question_id):
    question = get_object_or_404(Question, pk=question_id)
    return render(request, 'polls/results.html', {'question': question})

def vote(request, question_id):
    question = get_object_or_404(Question, pk=question_id)
    try:
        selected_choice = question.choice_set.get(pk=request.POST['choice'])
    except (KeyError, Choice.DoesNotExist):
        return render(request, 'polls/detail.html', {'question': question, 'error_message': "You didn\'t select a choice."})
    selected_choice.votes += 1
    selected_choice.save()
    return HttpResponseRedirect(reverse('polls:results', args = (question.id,)))

@login_required
def count(request):
    return render(request, 'polls/count.html')

def email_check(user):
    try:
        if user.email.endswith('@example.com'):
            return True
    except:
        return False

@user_passes_test(email_check)
def email_test(request):
    return HttpResponse('Test passed!')

@permission_required('polls.change_question')
def vote_test(request):
    return HttpResponse('Test passed!')