FROM python:3.7
ARG NODE_VER=12.18.3

# prepare node env
RUN curl -O https://nodejs.org/dist/v${NODE_VER}/node-v${NODE_VER}-linux-x64.tar.xz
RUN tar xf node-v${NODE_VER}-linux-x64.tar.xz -C /opt/
ENV PATH=$PATH:/opt/node-v${NODE_VER}-linux-x64/bin

# build game source
WORKDIR /usr/src/game-build
COPY ./game-project .
RUN npm install gulp-cli -g --registry https://registry.npm.taobao.org
RUN npm install --registry https://registry.npm.taobao.org
RUN gulp

# prepare flask env
WORKDIR /usr/src/app
COPY . .
RUN pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple

RUN cp -R /usr/src/game-build/dist static/light-world

CMD ["gunicorn", "app:app", "-c", "./gunicorn.conf.py"]