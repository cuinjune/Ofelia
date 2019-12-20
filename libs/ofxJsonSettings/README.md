# ofxJsonSettings

openFrameworks addon for providing a convenient, JSON-based settings manager

## Requirements

This addon depends on [ofxJSON](https://github.com/jefftimesten/ofxJSON).

## Setup

Clone this addon into your `openFrameworks/addons` folder.

```
cd path/to/openFrameworks/addons/

# if you don't have it yet
git clone https://github.com/jefftimesten/ofxJSON.git

git clone https://github.com/mattfelsen/ofxJsonSettings.git
```

## Usage

Include the header wherever you want to have the settings available. You don't need to instantiate this addon – a singleton instance will be created upon first use.

In your header file

`#include "ofxJsonSettings.h"`

In your cpp file...

#### Load

```
// Use Settings::get() to access the singleton instance
// The load() and save() methods will use "settings.json" if you don't specify a filename
Settings::get().load("data.json");
```

#### Save

If you want to save your settings, such as during app exit or on a gui button press...

```
Settings::get().save("data.json");
// Or disable the default pretty print with..
Settings::get().save("data.json", false);
```

#### Accessing settings

You can use the following methods to get your settings...

- `Settings::getString("key")`
- `Settings::getBool("key")`
- `Settings::getInt("key")`
- `Settings::getFloat("key")`
- `Settings::getDouble("key")`

```
ofSetFullscreen(Settings::getBool("fullscreen"));
```

You can also get nested keys like so...

```
heading.load(Settings::getString("fonts/heading/face"), Settings::getFloat("fonts/heading/size"));
body.load(Settings::getString("fonts/body/face"), Settings::getFloat("fonts/body/size"));
```

Accessing nested keys as shown above will parse/save json as such:

```
{
  "fonts" : {
    "body" : {
      "face": "font-body.ttf",
      "size": 16
    },
    "heading" : {
      "face": "font-heading.ttf",
      "size": 32
    }
  }
}
```

#### Updating or adding a setting...

Because all of the getters return references, you can also assign to them...

```
Settings::getFloat("x-pos") = 100;
Settings::getFloat("y-pos") = 100;
Settings::getFloat("size") = 50;
```

Or assign a nested structure...

```
Settings::getFloat("object/x") = 10;
Settings::getFloat("object/y") = 20;
```

#### Delimiter

By default, the delimiter is "/" but you can change it with...

```
Settings::get().setDelimiter("|")
```

#### Events

If you have classes that want to get notified of when the settings are loaded or saved, i.e. if you need to reload a font when the json file is loaded, you can...

```
ofAddListener(Settings::get().settingsLoaded, this, &ofApp::onSettingsLoaded);
ofAddListener(Settings::get().settingsSaved, this, &ofApp::onSettingsSaved);
```

#### More examples...

Set up a hypothetical websocket client

```
string host = Settings::getString("server/host");
int port = Settings::getInt("server/port");
bool reconnect = Settings::getBool("server/reconnect");
websocketClient.setup(host, port, reconnect)
```

Bind settings directly to a GUI that supports variable binding, like ofxUI or ofxDatGui

```
gui->addSlider("X Position", Settings::getFloat("x-pos"));
gui->addSlider("Y Position", Settings::getFloat("y-pos"));
gui->addSlider("Size", Settings::getFloat("size"));
```

If you're binding variables to a GUI, you'll want to load your settings file before setting up the GUI, and save back to a file on a button press or automatically when the app exits.

## Wait, why not just use ofxJSON directly?

A few reasons...

### Variable binding for GUIs

A few addons like [ofxUI](https://github.com/rezaali/ofxUI), [ofxDatGui](https://github.com/braitsch/ofxDatGui), and [ofxRemoteUI](https://github.com/armadillu/ofxRemoteUI) support binding variables to GUI elements so that they get updated when you drag a slider, for example. ofxJSON's getter methods, such as `json["key"].asString()` return values, not references, so you can't utilize variable binding. This addon's getters, such as `Settings::getString("key")`, return references so you can bind away!

### More concise syntax for nested keys

This may be subjective, but I find it easier to type slashes rather than lots of double-quotes and square brackets for lookups such as...

- ofxJSON: `json["fonts"]["heading"]["face"].asString()`
- ofxJsonSettings: `Settings::getString("fonts/heading/face")`
 
### Global access

This addon provides a singleton object so that you can access your settings from any file by including the `ofxJsonSettings.h` header file. This means you can do things like manage & manipulated GUI-bound settings in some top-level class (like ofApp) and have separate classes which can easily access those values. 
