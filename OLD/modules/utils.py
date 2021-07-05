import math

class Joint:
    def __init__(self, x, y, z, visibility):
        self.x: float = x
        self.y: float = y
        self.z: float = z
        self.visibility: float = visibility

class Dimension:
    def __init__(self, width:float=0, height:float=0) -> None:
        self.width:float = width
        self.height:float = height

    def getWidth(self) -> float:
        return self.width

    def getHeight(self) -> float:
        return self.height

    def setWidth(self, width:float=0) -> None:
        self.width = width

    def setHeight(self, height:float=0) -> None:
        self.height = height

    def __add__(self, o:any) -> any:
        if (isinstance(o, Dimension)):
            return Dimension(self.width+o.width, self.height+o.height)
        else:
            return Dimension(self.width+o, self.height+o)

    def __sub__(self, o:any) -> any:
        if (isinstance(o, Dimension)):
            return Dimension(self.width-o.width, self.height-o.height)
        else:
            return Dimension(self.width-o, self.height-o)

    def __mul__(self, o:any) -> any:
        if (isinstance(o, Dimension)):
            return Dimension(self.width*o.width, self.height*o.height)
        else:
            return Dimension(self.width*o, self.height*o)

    def __truediv__(self, o:any) -> any:
        if (isinstance(o, Dimension)):
            return Dimension(self.width/o.width, self.height/o.height)
        else:
            return Dimension(self.width/o, self.height/o)

    def __iadd__(self, o:any) -> None:
        if (isinstance(o, Dimension)):
            self.width += o.width
            self.height += o.height
        else:
            self.width += o
            self.height += o

    def __isub__(self, o:any) -> None:
        if (isinstance(o, Dimension)):
            self.width -= o.width
            self.height -= o.height
        else:
            self.width -= o
            self.height -= o

    def __imul__(self, o:any) -> None:
        if (isinstance(o, Dimension)):
            self.width *= o.width
            self.height *= o.height
        else:
            self.width *= o
            self.height *= o

    def __idiv__(self, o:any) -> None:
        if (isinstance(o, Dimension)):
            self.width /= o.width
            self.height /= o.height
        else:
            self.width /= o
            self.height /= o

    def __str__(self) -> str:
        return "Dimension("+self.width+" x "+self.height+")"

class Position:
    def __init__(self, x:float=0, y:float=0) -> None:
        self.x: float = x
        self.y: float = y

    def getX(self) -> float:
        return self.x

    def getY(self) -> float:
        return self.y

    def setX(self, x:float=0) -> None:
        self.x = x

    def setY(self, y:float=0) -> None:
        self.y = y

    def distance(self, other:any) -> float:
        return math.sqrt( (self.x-other.x)**2 + (self.y-other.y)**2 )

    def __add__(self, o:any) -> any:
        if (isinstance(o, Position)):
            return Position(self.x+o.x, self.y+o.y)
        else:
            return Position(self.x+o, self.y+o)

    def __sub__(self, o:any) -> any:
        if (isinstance(o, Position)):
            return Position(self.x-o.x, self.y-o.y)
        else:
            return Position(self.x-o, self.y-o)

    def __mul__(self, o:any) -> any:
        if (isinstance(o, Position)):
            return Position(self.x*o.x, self.y*o.y)
        else:
            return Position(self.x*o, self.y*o)

    def __truediv__(self, o:any) -> any:
        if (isinstance(o, Position)):
            return Position(self.x/o.x, self.y/o.y)
        else:
            return Position(self.x/o, self.y/o)

    def __iadd__(self, o:any) -> None:
        if (isinstance(o, Position)):
            self.x += o.x
            self.y += o.y
        else:
            self.x += o
            self.y += o

    def __isub__(self, o:any) -> None:
        if (isinstance(o, Position)):
            self.x -= o.x
            self.y -= o.y
        else:
            self.x -= o
            self.y -= o

    def __imul__(self, o:any) -> None:
        if (isinstance(o, Position)):
            self.x *= o.x
            self.y *= o.y
        else:
            self.x *= o
            self.y *= o

    def __idiv__(self, o:any) -> None:
        if (isinstance(o, Position)):
            self.x /= o.x
            self.y /= o.y
        else:
            self.x /= o
            self.y /= o

    def __str__(self) -> str:
        return "Position("+self.x+", "+self.y+")"

class Position3D:
    def __init__(self, x:float=0, y:float=0, z:float=0) -> None:
        self.x: float = x
        self.y: float = y
        self.z: float = z

    def getX(self) -> float:
        return self.x

    def getY(self) -> float:
        return self.y

    def getZ(self) -> float:
        return self.z

    def setX(self, x:float=0) -> None:
        self.x = x

    def setY(self, y:float=0) -> None:
        self.y = y

    def setZ(self, z:float=0) -> None:
        self.z = z

    def distance(self, other:any) -> float:
        return math.sqrt( (self.x-other.x)**2 + (self.y-other.y)**2 + (self.z-other.z)**2 )

    def __add__(self, o:any) -> any:
        if (isinstance(o, Position3D)):
            return Position3D(self.x+o.x, self.y+o.y, self.z+o.z)
        else:
            return Position3D(self.x+o, self.y+o, self.z+o)

    def __sub__(self, o:any) -> any:
        if (isinstance(o, Position3D)):
            return Position3D(self.x-o.x, self.y-o.y, self.z-o.z)
        else:
            return Position3D(self.x-o, self.y-o, self.z-o)

    def __mul__(self, o:any) -> any:
        if (isinstance(o, Position3D)):
            return Position3D(self.x*o.x, self.y*o.y, self.z*o.z)
        else:
            return Position3D(self.x*o, self.y*o, self.z*o)

    def __truediv__(self, o:any) -> any:
        if (isinstance(o, Position3D)):
            return Position3D(self.x/o.x, self.y/o.y, self.z/o.z)
        else:
            return Position3D(self.x/o, self.y/o, self.z/o)

    def __iadd__(self, o:any) -> None:
        if (isinstance(o, Position3D)):
            self.x += o.x
            self.y += o.y
            self.z += o.z
        else:
            self.x += o
            self.y += o
            self.z += o

    def __isub__(self, o:any) -> None:
        if (isinstance(o, Position3D)):
            self.x -= o.x
            self.y -= o.y
            self.z -= o.z
        else:
            self.x -= o
            self.y -= o
            self.z -= o

    def __imul__(self, o:any) -> None:
        if (isinstance(o, Position3D)):
            self.x *= o.x
            self.y *= o.y
            self.z *= o.z
        else:
            self.x *= o
            self.y *= o
            self.z *= o

    def __idiv__(self, o:any) -> None:
        if (isinstance(o, Position3D)):
            self.x /= o.x
            self.y /= o.y
            self.z /= o.z
        else:
            self.x /= o
            self.y /= o
            self.z /= o

    def __str__(self) -> str:
        return "Position3D("+self.x+", "+self.y+", "+self.z+")"