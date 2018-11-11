--[[
  nothing exciting here, just a bunch of stuff to see if the interpreter chokes
  on any of the following lines (aka something isn't wrapped correctly, etc)

  TODO: more comprehensive unit tests?
]] 

-- test that require is working
require "lib/test"

local font

----------------------------------------------------
function setup()
  print("script setup")

  of.setLogLevel(of.LOG_VERBOSE);
  of.log(of.LOG_VERBOSE, "blah blah blah "..tostring(123.4))

  of.setWindowTitle("function test")

  local testString = "/hello/this/is/a/fake/path"
  print("test: "..testString)

  -- splitString returns a wrapped C++ std::vector<string>
  local testSplitStrings = of.splitString(testString, "/")
  print("test split: "..tostring(testSplitStrings:size()))
  for i = 1, testSplitStrings:size()-1 do
    print("\t"..testSplitStrings[i])
  end

  local testJoinStrings = of.joinString(testSplitStrings, "/")
  print("test join: "..testJoinStrings)

  local style = of.Style()

  local rect = of.Rectangle()
  local m3x3 = of.Matrix3x3()
  local m4x4 = of.Matrix4x4()
  local quat = of.Quaternion()

  local v2f = of.Vec2f()
  local v3f = of.Vec3f()
  local v4f = of.Vec4f()

  local c = of.Color()
  c:set(127.0, 127.0, 255.0)
  print("c: "..c:getR().." "..tostring(c.g).." " ..tostring(c.b).." "..tostring(c.a))
  print(tostring(c))

  local pixels = of.Pixels()
  pixels:clear()

  local path = of.Path()
  path:clear()

  local polyline = of.Polyline()
  polyline:clear()

  of.TrueTypeFont.setGlobalDpi(96)
  font = of.TrueTypeFont()
  font:load("fonts/verdana.ttf", 16)
  print("font isLoaded: "..tostring(font:isLoaded()))
  print("font lineHeight: "..font:getLineHeight())

  local player = of.SoundPlayer()
  local stream = of.SoundStream()

  local node = of.Node()
  local mesh = of.Mesh()
  mesh:addVertex(glm.vec3(1, 2, 3))

  local light = of.Light()
  local material = of.Material()
  local fbo = of.Fbo()
  print("fbo allocated: "..tostring(fbo:isAllocated()))
  local vbo = of.Vbo()
  local vboMesh = of.VboMesh()
  local shader = of.Shader()

  local txtData = of.TextureData()
  local txt = of.Texture()
  txt:clear()
  print("texture allocated: "..tostring(txt:isAllocated()))

  if not of.Image then
    print "of.Image function doesn't exist"
  end
  local img = of.Image()

  -- GL type defines added by the swig interface (don't exist in OF)
  print("of.CLAMP_TO_EDGE: "..string.format("0x%X", of.CLAMP_TO_EDGE))
  print("of.CLAMP_TO_BORDER: "..string.format("0x%X", of.CLAMP_TO_BORDER))
  print("of.REPEAT: "..string.format("0x%X", of.REPEAT))
  print("of.MIRRORED_REPEAT: "..string.format("0x%X", of.MIRRORED_REPEAT))
  print("of.TEXTURE_LUMINANCE: "..string.format("0x%X", of.TEXTURE_LUMINANCE))
  print("of.TEXTURE_RGB: "..string.format("0x%X", of.TEXTURE_RGB))
  print("of.TEXTURE_RGBA: "..string.format("0x%X", of.TEXTURE_RGBA))
  print("of.NEAREST: "..string.format("0x%X", of.NEAREST))
  print("of.LINEAR: "..string.format("0x%X", of.LINEAR))

  -- function loaded from separate script via require
  requireTest()

  -- size_t
  local haystack = "ka12ka34ka56ka"
  print("'ka' is found "..of.stringTimesInString(haystack, "ka").." times in '"..haystack.."'")

  -- uint_64t
  print("elapsed millis: "..of.getElapsedTimeMillis())

  -- util stuff
  of.restoreWorkingDirectoryToDefault()

  -- of.Buffer, test binary data in strings
  local buffer = of.Buffer("hello\0world")
  print("buffer size (11): "..buffer:size())
  buffer:clear()
  print("buffer size (0): "..buffer:size())
  buffer:set("abc\0de")
  print("buffer size (6): "..buffer:size())
  buffer:append("fg\0hij")
  print("buffer size (12): "..buffer:size())
  local data = buffer:getData()
  if data then
    io.write("buffer data (hex): ")
    for i = 1,#data do
      io.write(string.format("%.2X ", string.byte(data:sub(i,i))))
    end
    io.write("\n")
  end
  buffer:set("foo bar")
  print("buffer size (7): "..buffer:size())
  print("buffer text: "..buffer:getText())

  -- of.FilePath
  print("filepath current dir: "..of.FilePath.getCurrentWorkingDirectory())
  print("filepath joining /Users/foo with bar.txt: "..of.FilePath.join("/Users/foo", "bar.txt"))

  -- of.File
  local file = of.File("tests.lua")

  print("file path: "..file:path())
  print("file extension: "..file:getExtension())
  print("file name: "..file:getFileName())
  print("file basename: "..file:getBaseName())
  print("file enclosing directory: "..file:getEnclosingDirectory())
  print("file absolute path: "..file:getAbsolutePath())

  print("file can read?: "..tostring(file:canRead()))
  print("file can write?: "..tostring(file:canWrite()))
  print("file can execute?: "..tostring(file:canExecute()))

  print("file is file?: "..tostring(file:isFile()))
  print("file is link?: "..tostring(file:isLink()))
  print("file is directory?: "..tostring(file:isDirectory()))
  print("file is device?: "..tostring(file:isDevice()))
  print("file is hidden?: "..tostring(file:isHidden()))

  print("file size: "..file:getSize())

  -- of.Directory
  local dir = of.Directory("./")
  print("dir absolute path: "..dir:getAbsolutePath())

  -- glm vector types
  local point = of.Point(10, 20)
  print("point: "..point.x.." "..point.y)
  point.x = point.x + 10
  point.y = point.y + 10
  print("point+10: "..point.x.." "..point.y)
  print("point len: "..point:length())

  touch = of.TouchEventArgs(of.TouchEventArgs.up, 10, 20, 123)
  print(swig_type(touch))
  print("touch: "..touch.x.." "..touch.y)

  -- glm types
  local pos = glm.vec2(of.getWidth()/2 - 50, of.getHeight()/2 - 50)
  print("pos "..tostring(pos).." is a "..swig_type(pos))
  pos.x = 30
  pos.y = 40
  print("pos x:"..pos.x.." y:"..pos.y)
  print("pos[1]:"..pos[1].." pos[2]:"..pos[2])
  print("glm.pi: "..glm.pi())

  local v1 = glm.vec3(1, 2, 3)
  local v2 = glm.vec3(4, 5, 6)
  print("v1: "..tostring(v1))
  print("v2: "..tostring(v2))
  print("v1 + v2: "..tostring(v1 + v2))
  print("v1 dot v2: "..glm.dot(v1, v2))

  local v3 = glm.vec4()
  local m3 = glm.mat3()
  local m4 = glm.mat4()
  local q = glm.quat(0, 9, 8, 7)
  print("quat: "..tostring(q))

end

function update()
end

function draw()
  of.drawBitmapString("tests", 20, 20)

  --local red = of.Color.red
  --local gray = of.Color.lightGray
  --print("red: "..tostring(red))

  -- see if the named color class leaks memory...
  of.fill()
  --of.setColor(gray)
  of.drawRectangle(glm.vec2(of.getWidth()/2 - 50, of.getHeight()/2 - 50), 100, 100)
  of.drawRectangle(touch, 20, 20)
  of.drawRectangle(of.Vec2f(100, 100):vec2(), 10, 10)

  -- see if bounding box rect leaks memory...
  --of.setColor(red)
 -- local box = font:getStringBoundingBox("hello world", 0, 0)
  --font:drawString("hello world", of.getWidth()/2 - box.width/2, of.getHeight()/2)
  --print("box before:"..tostring(box))

  -- box = nil
  -- red = nil
  -- gray = nil
  --print("box after :"..tostring(box))
end
