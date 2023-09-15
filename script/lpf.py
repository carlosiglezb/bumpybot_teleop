from numpy import zeros, asarray, float64
from geometry_msgs.msg import Vector3

def vector3_convertor(v3):
    output = zeros((3,1))
    output[0], output[1], output[2] = v3.x, v3.y, v3.z
    return asarray(output, dtype = float64)

class vel_smoother:
    def __init__(self):
        self.linear, self.angular= zeros((3,1)), zeros((3,1))
    
    def lpf(self, v3_linear, v3_angular):
        input_linear,input_angular = vector3_convertor(v3_linear),vector3_convertor(v3_angular)
        alpha = [0,0]
        if (input_linear==0).all() == True:
            alpha[0] = 0.01
        else:
            alpha[0] = 0.001
        if (input_angular==0).all() == True:
            alpha[1] = 0.01
        else:
            alpha[1] = 0.001      
        new_linear = alpha[0]*input_linear + (1-alpha[0])*self.linear
        new_angular = alpha[1]*input_angular + (1-alpha[1])*self.angular
        out_linear, out_angular = Vector3(float(new_linear[0]),float(new_linear[1]),float(new_linear[2])), Vector3(float(new_angular[0]),float(new_angular[1]),float(new_angular[2]))
        self.linear, self.angular = new_linear, new_angular
        return  out_linear, out_angular
