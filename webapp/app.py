from flask import Flask, render_template, url_for, request
app = Flask(__name__)

@app.route("/")
@app.route("/<name>")
def hello(name = None):
    baseUrl = request.url_root[:-1]
    return render_template("index.html", name = name, vsSrc = baseUrl + url_for("static", filename= "glsrc/MultiPoint.vert"), fsSrc = baseUrl + url_for("static", filename= "glsrc/MultiPoint.frag"))

if __name__ == "__main__":
    app.run()