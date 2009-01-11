from PIL import Image


im = Image.open("man.png")
parts = im.split()
new = Image.merge("RGB",(parts[0],parts[1],parts[2]))

matrix =  ( 0.299,0.587,0.114,0)

matrix =  ( 0.7,0,0.3,0,
            0,0.7,0.3,0,
            0,0,0,0
        )


out = new.convert("RGB",matrix)
out.save("out.png","PNG")
gamma = [0,0,1,2,3,5,9,15]

doc='''
uint8_t  fb1[2][16][16] = {
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    },
    {
'''


doc='''
uint8_t  fb1[2][16][16] = {
    {
''' 

im      = out
comp    = 0
bits    = 3
sizex   = 16
sizey   = 16
range   = 16
padx    = sizex * bits
pady    = sizey * bits
data    = im.tostring()

for y in xrange(0,pady,bits):
    doc += "        {"
    for  x in xrange(0,padx,bits):
        v = ord(data[x   + ( y * sizey ) + comp]) / 32
        doc += "0x%02x,"  % ( gamma[v]  )
    doc = doc[:-1]
    doc += "},\n"

comp = 1
doc += "    },{\n"


for y in xrange(0,pady,bits):
    doc += "        {"
    for  x in xrange(0,padx,bits):
        v = ord(data[x   + ( y * sizey ) + comp]) / 32
        doc += "0x%02x,"  % ( gamma[v]  )
    doc = doc[:-1]
    doc += "},\n"



doc += "    }\n};\n"
 
print doc


