APP_NAME=lightengine

function stopapp() {
    echo stopping ${APP_NAME}
    targetid=`docker container ls | awk '{if($2=="'${APP_NAME}'"){print $1}}'`
    docker stop ${targetid}
}

function startapp() {
    echo starting ${APP_NAME}
    docker run -dp 80:80 ${APP_NAME}
}

function buildapp() {
    echo building ${APP_NAME}
    docker build -t ${APP_NAME} .
}

function cleannoneimg() {
    echo cleaning useless images
    idlist=`docker images | awk '{if($1=="<none>"){print $3}}'`
    for id in ${idlist}
    {
        docker image rm $id -f
    }
}

stopapp
buildapp
startapp
cleannoneimg