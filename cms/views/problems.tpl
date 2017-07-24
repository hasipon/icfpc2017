% include('head.tpl')

<div class="container">
<h1>Problems</h1>

<table class="table table-condensed table-striped">
<tr>
    <th class="text-center">problem_id</th>
    <th class="text-center">ranking</th>
    <th class="text-center">resemblance</th>
    <th class="text-center">solution_size</th>
    <th class="text-center">top_resemblance</th>
    <th class="text-center">complete_teams</th>
    <th class="text-center">problem_size</th>
    <th class="text-center">owner</th>
    <th class="text-center">owner_solution_size</th>
    <th class="text-center">image</th>
    <th class="text-center">official</th>
</tr>
% for p in problems:
    </td>
    <td class="text-center">{{p['problem_size']}}</td>
    <td class="text-center">{{p['owner']}}</td>
    <td class="text-center">{{p['solution_size']}}</td>
    <td class="text-center">
    <a href="/problems/{{p['problem_id']}}.png">image</a>
    </td>

    <td class="text-center">
    <a href="http://2016sv.icfpcontest.org/problem/view/{{p['problem_id']}}">detail</a>
    </td>
</tr>
% end

</div>

% include('tail.tpl')
