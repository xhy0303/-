from flask import Flask as _Flask,jsonify
from flask import render_template
from flask.json import JSONEncoder as _JSONEncoder
from jieba.analyse import extract_tags
import decimal
import utils
import string

class JSONEncoder(_JSONEncoder):
        def default(self, o):
            if isinstance(o, decimal.Decimal):
                return float(o)
            super(_JSONEncoder, self).default(o)

class Flask(_Flask):
    json_encoder = JSONEncoder


app = Flask(__name__)



@app.route('/')
def hello_word3():
    return render_template("main.html")

@app.route('/ajax',methods=["get","post"])
def hello_word4():
    return '10000'

@app.route('/time')
def get_time():
    return utils.get_time()

@app.route('/c1')
def get_c1_data():
    data = utils.get_c1_data()
    return jsonify({"confirm":data[0],"suspect":data[1],"heal":data[2],"dead":data[3]})

@app.route('/c2')
def get_c2_data():
    res = []
    for tup in utils.get_c2_data():
        res.append({"name":tup[0],"value":int(tup[1])})
    return jsonify({"data":res})

@app.route('/l1')
def get_l1_data():
    data = utils.get_l1_data()
    day,confirm,imported_case,heal,dead = [],[],[],[],[]
    for a,b,c,d,e in data[7:]:
        day.append(a.strftime("%m-%d"))
        confirm.append(b)
        imported_case.append(c)
        heal.append(d)
        dead.append(e)
    return jsonify({"day":day,"confirm":confirm,"imported_case":imported_case,"heal":heal,"dead":dead})

@app.route('/l2')
def get_l2_data():
    data = utils.get_l2_data()
    day,now_confirm,now_severe,suspect = [],[],[],[]
    for a,b,c,d in data[7:]:
        day.append(a.strftime("%m-%d"))
        now_confirm.append(b)
        now_severe.append(c)
        suspect.append(d)
    return jsonify({"day":day,"now_confirm":now_confirm,"now_severe":now_severe,"suspect":suspect})

@app.route('/r1')
def get_r1_data():
    data = utils.get_r1_data()
    city = []
    confirm_add = []
    for k,v in data:
        city.append(k)
        confirm_add.append(int(v))
    return jsonify({"city": city,"confirm_add": confirm_add})

@app.route('/r2')
def get_r2_data():
    data = utils.get_r2_data()
    city = []
    confirm_add = []
    for k,v in data:
        city.append(k)
        confirm_add.append(int(v))
    return jsonify({"city": city, "confirm_add": confirm_add})

if __name__ == '__main__':
    app.run(host="0.0.0.0",port=5000)
