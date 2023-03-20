class MetaConst(type):
    def __getattr__(cls, key):
        return cls[key]

    def __setattr__(cls, key, value):
        raise TypeError


class Const(object, metaclass=MetaConst):
    def __getattr__(self, name):
        return self[name]

    def __setattr__(self, name, value):
        raise TypeError


class Constants(Const):
    URL = "ws://localhost:8083"
    RANGE = 100000
    PATH = "./bin/"
    SYNC_SERVER_NAME = "sync_server"
    ASYNC_SERVER_NAME = "async_server"
