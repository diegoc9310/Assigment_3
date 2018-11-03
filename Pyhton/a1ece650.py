import sys
import re
import hashlib
# YOUR CODE GOES HERE
class Line(object):
    def __init__ (self, src, dst):
        self.src = src
        self.dst = dst

    def __str__(self):
        return   '<' +str(self.src.index) + ',' + str(self.dst.index) + '>' 

class Point(object):
    def __init__ (self, x, y,name):
        self.x = float(x)
        self.y = float(y)
        self.name = str(name)
        self.index = hashlib.sha1('(' + str(pp(self.x)) + ',' + str(pp(self.y))+ ')'.encode("UTF-8")).hexdigest()[:4]
    def __str__ (self):
        return str(self.index)+':  '+'(' + str(pp(self.x)) + ',' + str(pp(self.y))+ ')'
def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)
class Distance(object):
    def __init__(self, index, distance):
        self.index = index
        self.distance = distance

    @classmethod
    def sort_key(cls, key):
        if key == 'distance':
            return lambda obj: obj.distance
        elif key == 'index':
            return lambda obj: obj.index


def out_of_range(x1, y1, x2, y2, x3, y3, x4, y4, xcoor, ycoor,name,test):
    xbig    = first_bigger_second(x1,x2)
    ybig    = first_bigger_second(y1,y2)
    xbig2   = first_bigger_second(x3,x4)
    ybig2   = first_bigger_second(y3,y4)
    xsmall  = first_smaller_second(x1,x2)
    ysmall  = first_smaller_second(y1,y2)
    xsmall2 = first_smaller_second(x3,x4)
    ysmall2 = first_smaller_second(y3,y4)

    if (xcoor>xbig) or (xcoor<xsmall) or (xcoor>xbig2) or (xcoor<xsmall2) :
      return False

    if (ycoor>ybig) or (ycoor<ysmall) or (ycoor>ybig2) or (ycoor<ysmall2) :
      return False

    return Point(xcoor, ycoor, name)
  
def intersect (l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)

    if (xden != 0) & (yden != 0):
      xcoor =  xnum / xden
      ycoor =  ynum / yden
    else:
      return False

    return out_of_range(x1, y1, x2, y2, x3, y3, x4, y4, xcoor, ycoor,"intersect",0)

def first_bigger_second ( first,  second):
  if (first >= second):
    return first;
  if (first < second):
    return second

def first_smaller_second ( first,  second):
  if (first >= second):
    return second;
  if (first < second):
    return first

def cross_product(p1, p2, p3):        
  x1 = p3.x - p1.x;
  y1 = p3.y - p1.y;

  x2 = p2.x - p1.x;
  y2 = p2.y - p1.y;
  cross = x1 * y2 - y1 * x2;

  if cross ==0:
    xbig    = first_bigger_second(p1.x,p2.x)
    ybig    = first_bigger_second(p1.y,p2.y)
    xsmall  = first_smaller_second(p1.x,p2.x)
    ysmall  = first_smaller_second(p1.y,p2.y)
    if (p3.x>xbig) or (p3.x<xsmall)  :
      return False
    if (p3.y>ybig) or (p3.y<ysmall) :
      return False
    else:
      return p3
  else:
    return False

##########################################
# Search Street Name
# Returns True if name foun false if not
# parameter: List of lists of point objects 
#########################################
def search_street_name(street_list,street_name):
  new_street="False"
  for i in range(0,(len(street_list))):
    if street_name.lower()==street_list[i][0].name.lower():
      new_street=i
  return new_street

##########################################
# COMMAND PARSER MODULE
# a->Add c->Change r->Remove g->Graph
# parser state 9 Represents that an 
# appropiate command has been recieved
# parameter: String input
#########################################
###########PARSER STATE MACHINE###########
def Parser(line):
    parser_state=0
    test_points=[]
    name=""
    sign_flag=0
    number_flag=0
    space_flag=0
    pointx=""
    pointy=""
    command=0
    point_list=[]
    for len_input in range(0,len(line),1):  
#########State 0#########
      if parser_state==0:
        x=line[len_input]
        if x ==" ":
          pass
        elif x.isalpha():
          if x=="a" or x=="c":
            command=x            
            parser_state = 1 #Go Next
          elif x=="r":
            command=x
            parser_state = 11 #Go Next
          elif x=="g":
            command=x        
            if len_input==len(line)-2:
              parser_state = 9 #Go Next
            else:
             parser_state = 14 #Go Next 
          else:
            sys.stderr.write( "Error: Command not recognized.\n" )
            break 

        else:
          sys.stderr.write( "Error: Command not recognized.\n" )
          break         
########ADD/CHANGE COMMAND########
###State 1###
      elif parser_state==1:
        if line[len_input]==" ":
          parser_state = 2 #Go Next 
        else:
          if len_input==len(line)-1:
            sys.stderr.write( "Error: Missing required arguments.\n" )
          else:
            sys.stderr.write( "Error: Add a space between command and arguments.\n" )
          break
###State 2###
      elif parser_state==2:
        if line[len_input]==" ":
          pass
        elif line[len_input]=='"':
          parser_state = 3 #Go Next
          name=""
        else:
          sys.stderr.write( "Error: needed name argument use format: 'Street Name'.\n" )
          break
###State 3###
      elif parser_state==3:
        x=line[len_input]
        if x.isalpha():
          name+=line[len_input]
        elif x.isdigit():
          sys.stderr.write( "Error: Do not enter numbers or special characters in Street Name.\n" )
          break
        elif line[len_input]!='"' and line[len_input]!=' ':
          sys.stderr.write( "Error: wrong command format.\n" )
          break
        elif line[len_input]=='"':
          if len(name)>=1:
            parser_state = 4 #Go Next
          else:
            sys.stderr.write( "Error: Name not found.\n" )
            break
        elif line[len_input]==" ":
          name+=line[len_input] 
        else:
          sys.stderr.write( "Error: Do not enter numbers or special characters in Street Name.\n" )
          break
###State 4###
      elif parser_state==4:
        
        if line[len_input]==" ":
          parser_state = 5 #Go Next 
        else:
          if len_input==len(line)-1:
            sys.stderr.write( "Error: Missing required arguments.\n" )
          else:
            sys.stderr.write("Error: Space nedded between 'Name'argument and coordinates (x,y).\n" )
          break
###State 5###
      elif parser_state==5:
        if line[len_input]==" ":
          pass
        elif line[len_input]=="(":
          parser_state=6 #Go Next
        else:
          sys.stderr.write( "Error: needed coordinates (x,y).\n" )
          break
###State 6###

      elif parser_state==6:
        if line[len_input]==" ":
          if number_flag==1 or sign_flag==1:
           space_flag=1
          else:
            pass
        elif line[len_input]=="-":
          if sign_flag==0 and space_flag==0:
            pointx+=line[len_input]
            sign_flag=1
          else:
            sys.stderr.write( "Error: wrong coordinate format.\n" )
            break
        elif line[len_input].isdigit():
          if space_flag==0:
            pointx+=line[len_input] 
            number_flag=1
            sign_flag=1
          else:
            sys.stderr.write( "Error: wrong coordinate format.\n" )
            break
        elif line[len_input]==",":
          if(number_flag==1):
            parser_state = 7 #Go Next 
            sign_flag=0
            space_flag=0
            number_flag=0
          else:
            sys.stderr.write( "Error: wrong coordinate format.\n" )
            break
        else:
          sys.stderr.write( "Error: wrong coordinate format.\n" )
          break
###State 7###
      elif parser_state==7:
        if line[len_input]==" ":
          if number_flag==1 or sign_flag==1:
           space_flag=1
          else:
            pass
        elif len_input==len(line)-2:
          if line[len_input]==")":
            if(number_flag==1):
              test_points.append('('+str(pointx)+','+str(pointy)+')')
              pointx=""
              pointy=""
              sign_flag=0
              space_flag=0
              number_flag=0
              parser_state = 9 #Go Next
            else:
              sys.stderr.write( "Error: wrong coordinate format.\n" )
              break 
          else:
            sys.stderr.write( "Error: wrong coordinate format.\n" )
            break
        else:
          if line[len_input]=="-":
            if sign_flag==0:
              pointy+=line[len_input]
              sign_flag=1
            else:
              sys.stderr.write( "Error: wrong coordinate format.\n" )
              break
          elif line[len_input].isdigit():
            if space_flag==0:
              pointy+=line[len_input] 
              number_flag=1
              sign_flag=1
            else:
              sys.stderr.write( "Error: wrong coordinate format.\n'" )
              break
          elif line[len_input]==")":
            if(number_flag==1):
              parser_state = 8 #Go Next 
              test_points.append('('+str(pointx)+','+str(pointy)+')')
              pointx=""
              pointy=""
              sign_flag=0
              space_flag=0
              number_flag=0
            else:
              sys.stderr.write( "Error: wrong coordinate format.\n" )
              break
          else:
            sys.stderr.write( "Error: wrong coordinate format.\n" )
            break
###State 8###  
      elif parser_state==8:
        if len_input==len(line)-2:
          if line[len_input]==" ":
             parser_state = 9 #Go Next 
          else:
            sys.stderr.write("Error: Wrong coordinate format.\n" )
        else:
          if line[len_input]==" ":
            pass
          elif line[len_input]=="(":
            if len_input!=len(line)-1:
              parser_state = 6 #Go Next 
              sign_flag=0
              space_flag=0
              number_flag=0
          else: 
            sys.stderr.write( "Error: Wrong coordinate format.\n" )
            break
######ADD/CHANGE COMMAND END######
##########REMOVE COMMAND##########
###State 11###
      elif parser_state==11:
        if line[len_input]==" ":
          parser_state = 12 #Go Next 
        else:
          if len_input==len(line)-1:
            sys.stderr.write( "Error: required 'Name' argument.\n" )
          else:
            sys.stderr.write( "Error: required space separation between command and argument.\n" )
          break
###State 12###
      elif parser_state==12:
        if line[len_input]==" ":
          pass
        elif line[len_input]=='"':
          parser_state = 13 #Go Next
          name=""
        else:
          sys.stderr.write( "Error: required 'Name'argument.\n" )
          break
###State 13###
      elif parser_state==13:
        x=line[len_input]
        if x.isalpha():
          name+=line[len_input]
        elif x.isdigit():
          sys.stderr.write( "Error: Do not enter numbers or special characters in Street Name.\n" )
          break
        elif line[len_input]!='"' and line[len_input]!=' ':
          sys.stderr.write( "Error: wrong command format.\n" )
          break
        elif line[len_input]=='"':
          if len_input==len(line)-2:
            if len(name)>=1:
              parser_state = 9 #Go Next
            
            else:
              sys.stderr.write( "Error: Name not found.\n" )
              break

          elif len(name)>=1:
            parser_state = 14 #Go Next
          else:
            sys.stderr.write( "Error: Name not found.\n" )
            break

        elif line[len_input]==" ":
          name+=line[len_input] 
        else:
          sys.stderr.write( "Error: Do not enter numbers or special characters in Street Name.\n" )
          break
###State 14### This state is shared between graph and remove command
      elif parser_state==14:
        error_flag=0
        for a in range(len_input,len(line)-1,1):
          if line[a]==" ":
            pass
          else:
            error_flag=1
        if error_flag==0:
          parser_state = 9 #Go Next 
          break
        else:
          if command=='r':
            sys.stderr.write("Error: Wrong command format.\n" )
          elif command=='g':
            if line[len_input]!=" ":
              sys.stderr.write( "Error: Wrong command format.\n" )
            else:
              sys.stderr.write("Error: Command recieves no arguments.\n" )
          else:
            sys.stderr.write( "Error: Wrong command format.\n" )
          break
########REMOVE COMMAND END########
#########PARSER STATE MACHINE END#########
    if parser_state==9:
      for i in xrange(0,len(test_points),1):
        points = re.findall("[-+]?\d+[\.]?\d*[eE]?[-+]?\d*", test_points[i])
        p=Point(points[0],points[1],name)
        point_list.append(p)
      return command,name,point_list
    else:
      return False

##########################################
# GRAPHING MODULE
# Computes and prints Edges and vertexes
# parameter: List of List of Street points
#########################################
def graph_calculator(street_list):
  delete_list=[]
  intercept_list=[]
  point_intercept_list=[]
  street_intercept_list=[]
  intercept_found=False
  point_test=0
  dist_list=[]
  edge_list=[]
  sorted_street_list=[]
  vertex_list=[]
  foud_flag=0

  for i in xrange(0,(len(street_list))):
    x= len(street_list[i])-1
    for j in xrange(0,x,1):
      for k in xrange(0,len(street_list)):
        if k!=i:
          for l in xrange(0,(len(street_list[k])-1)):
            line_1=Line(street_list[i][j],street_list[i][j+1])
            line_2=Line(street_list[k][l],street_list[k][l+1])
            point_test= intersect(line_1, line_2)
            if point_test!= False:
              intercept_found=True
              if(len(intercept_list)>0):
                for a in range(0,(len(intercept_list))):
                  if (intercept_list[a].x!=point_test.x) or (intercept_list[a].y!=point_test.y):
                    intercept_list.append(point_test)
                    foud_flag=1
              else:
                intercept_list.append(point_test)
                foud_flag=1
            else:
              p1=cross_product(street_list[i][j],street_list[i][j+1],street_list[k][l])
              if p1!= False:
                intercept_found=True
                intercept_list.append(street_list[k][l])
                foud_flag=1
                point_test=True
              p2=cross_product(street_list[i][j],street_list[i][j+1],street_list[k][l+1])
              if p2!= False:
                intercept_found=True
                street_list[k][l+1].name="intersect"
                intercept_list.append(street_list[k][l+1])
                foud_flag=1
                point_test=True

      if foud_flag!= 0:
        ################################################################################# 
        #Find index of repeated intersects
        delete_list[:]=[]
        for d in range(0,(len(intercept_list))):
          for c in range(1+d,(len(intercept_list))):
            if (intercept_list[d].x==intercept_list[c].x) and(intercept_list[d].y==intercept_list[c].y):
              if c not in delete_list:
                delete_list.append(c)
        delete_list = sorted(delete_list)

        #Delete repeated intersects 
        for f in xrange(0,(len(delete_list))):
          de= delete_list[f]-f
          del intercept_list[de]  

        if(len(intercept_list)>1):
          #Order street values by distance from starting point 
          
          dist_list[:]=[]
          for q in range(0,(len(intercept_list))):
            xdif = intercept_list[q].x-street_list[i][j].x
            ydif = intercept_list[q].y-street_list[i][j].y
            new1= ((xdif*xdif)+(ydif*ydif))**(.5)
            dist_list.append(Distance(q,new1))
          dist_list.sort(key=Distance.sort_key('distance'))
        
          point_intercept_list.append( street_list[i][j]  )
          for m in range(0,(len(dist_list))):
            x=dist_list[m].index
            point_intercept_list.append(intercept_list[x])
          
          point_intercept_list.append(  street_list[i][j+1] )

        else:
          point_intercept_list.append( street_list[i][j]  )       
          for z in range(0,(len(intercept_list))):
            point_intercept_list.append(intercept_list[z])
          point_intercept_list.append(  street_list[i][j+1] )

        intercept_list[:]=[]
        point_test=0
        foud_flag=0
          #################################################################################           
    if intercept_found==True:
      street_intercept_list.append(point_intercept_list[:])
      intercept_found=False
    point_intercept_list[:]=[]
          
#find index of repeated values in the streets and delete them 
  for x in range(0,(len(street_intercept_list))):
    delete_list[:]=[]
    for a in range(0,(len(street_intercept_list[x]))):
      for b in range(1+a,(len(street_intercept_list[x]))):
        if (street_intercept_list[x][a].x==street_intercept_list[x][b].x) and(street_intercept_list[x][a].y==street_intercept_list[x][b].y):
          if b not in delete_list:
            delete_list.append(b)
    delete_list = sorted(delete_list)
    for i in xrange(0,(len(delete_list))):
      de= delete_list[i]-i
      del street_intercept_list[x][de]


#Generate Edge List from ordered List   
  for a in range(0,(len(street_intercept_list))):
    for b in range(0,(len(street_intercept_list[a])-1)):
      if street_intercept_list[a][b].name==street_intercept_list[a][b+1].name:
        if street_intercept_list[a][b].name!="intersect":
          pass ## no intercept not considered edge
        else:
          edge_list.append(Line(street_intercept_list[a][b], street_intercept_list[a][b+1]))
      else:
        edge_list.append(Line(street_intercept_list[a][b], street_intercept_list[a][b+1]))

#Save all Vertexes from edge list before filtering
  for a in range(0,(len(edge_list))):
    vertex_list.append(edge_list[a].src)
    vertex_list.append(edge_list[a].dst)

#Find index of repeated Vertexes
  delete_list[:]=[]
  for a in range(0,(len(vertex_list))):
    for b in range(1+a,(len(vertex_list))):
      if (vertex_list[a].x==vertex_list[b].x) and(vertex_list[a].y==vertex_list[b].y):
        if b not in delete_list:
          delete_list.append(b)
  delete_list = sorted(delete_list)

#Delete repeated Vertexes 
  for i in xrange(0,(len(delete_list))):
    de= delete_list[i]-i
    del vertex_list[de]
       
#Print Graph 
  sys.stdout.write( "V = {\n" )
  for i in xrange(0,(len(vertex_list))):
    sys.stdout.write(str(vertex_list[i])+"\n")
  sys.stdout.write("}\n")
  sys.stdout.write( "E = {\n")
  for i in xrange(0,(len(edge_list))):
    if i==len(edge_list)-1:
      sys.stdout.write(str(edge_list[i])+"\n")
    else:
      sys.stdout.write( str(edge_list[i])+",\n")
  sys.stdout.write("}\n")

def main():

  point_list=[]
  street_list=[]
  while True:
    line = sys.stdin.readline()
    input_data=Parser(line)
    if input_data!=False:

########Extract Input Data########
      input_data=list(input_data)
      command = input_data[0]
      street_name = input_data[1]
      point_list = input_data[2][:]

########Add Command Interpretation########
      if command=='a':

        if len(street_list)>=1:
          name_found=search_street_name(street_list,street_name)
          if name_found=="False":
            if len(point_list)>1:
              street_list.append(point_list[:])
            else:
              sys.stderr.write( "Error: Streets need to contain at least two point coordinates\n")
          else:
            sys.stderr.write( "Error: this street has all ready been registered to change it use the c command\n")
        else:
          if len(point_list)>1:
            street_list.append(point_list[:])
          else:
            sys.stderr.write( "Error: Streets need to contain at least two point coordinates\n")

########Change Command Interpretation#####
      elif command=='c':
        if len(street_list)>=1:
          name_found=search_street_name(street_list,street_name)
          if name_found=="False":
            sys.stderr.write( "Error: Street not found\n")
          else:
            if len(point_list)>1:
              del street_list[name_found]
              street_list.append(point_list[:])
            else:
              sys.stderr.write( "Error: Streets need to contain at least two point coordinates\n")
        else:
          sys.stderr.write( "Error: currently there are no added Streets\n")

########Remove Command Interpretation#####
      elif command=='r':
        if len(street_list)>=1:
          name_found=search_street_name(street_list,street_name)
          if name_found=="False":
            sys.stderr.write( "Error: Street not found\n")
          else:
            del street_list[name_found]
        else:
          sys.stderr.write( "Error: currently there are no added Streets\n")
        pass

########Graph Command Interpretation######
      elif command=='g':
        if len(street_list)>=1:
          graph_calculator(street_list)
        else:
          sys.stderr.write( "Error: currently there are no added Streets\n")
   
  sys.exit(0)

if __name__ == '__main__':
  while True:
    main()