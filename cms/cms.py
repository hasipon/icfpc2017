#!/usr/bin/env python3
import json
import time
import glob
import os
import subprocess
from bottle import route, template, request, static_file, redirect, auth_basic, error, response, get, abort, run, debug, default_app

repo = os.path.dirname((os.path.dirname(os.path.abspath(__file__))))
www = repo + '/cms/static'

"""
Static Routing
"""
@route('/static/<path:path>')
def img(file_name):
    return static_file(path, root=www)

"""
@route('/problems/<file_name>')
def route_problems(file_name):
    return static_file(file_name, problems)

@route('/visualizer/<file_name>')
def route_problems(file_name):
    return static_file(file_name, visualizer)
"""

"""
Public Page Routing
"""
@route('/')
def title():
    json_data = ''
    with open(repo + "/problems.json") as f:
        json_data = f.read()
    data = json.loads(json_data)
    probs = data['problems']
    return template('problems', problems=probs)

@route('/gitstatus')
def git_status():
    output = ""
    try:
        output += subprocess.check_output(["git", "status"]).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return template('output', output=output)

@route('/pull')
def pull():
    try:
        output = subprocess.check_output(["git", "pull", "origin", "master"]).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return template('output', output=output)

@route('/submit-solution')
def solution_submit_get():
    return template('submit')

"""
@route('/submit-solution', method='POST')
def solution_submit_post():
    problem_id = request.forms.get('problem_id')
    solution = request.forms.get('solution')
    solution.replace('\r\n','\n')
    filename = "/tmp/" + problem_id + "-" + str(time.time())
    with open(filename, 'w') as f:
        f.write(solution)
    output = subprocess.check_output([repo + "/solution-submit", problem_id, filename])
    os.remove(filename)
    return template('output', output=output)


@route('/submit-problem/<publish_time>', method='POST')
def problem_submit_post(publish_time):
    solution = request.forms.get('solution')
    solution.replace('\r\n','\n')
    filename = "/tmp/" + publish_time + "-" + str(time.time())
    with open(filename, 'w') as f:
        f.write(solution)
    output = subprocess.check_output([repo + "/problem-submit", publish_time, filename])
    os.remove(filename)
    return template('output', output=output)
"""

run(default_app(), host='0.0.0.0', port=5000, debug=True, reloader=True, server='cheroot')

