# SystemC Viterbi

This project features a SystemC implementation of a Viterbi encoder and decoder.

## Documentation

The documentation is available
[here](http://sc-viterbi.readthedocs.io/en/latest/). The documentation contains
development guides as well as the design of the Viterbi encoder and decoder.

### Build the documentation

The documentation of this project was developed using
[Sphinx](http://www.sphinx-doc.org/en/stable/). To build the documentation we
recommend to create a ```Python``` virtual environment first by running;

```
cd <project-root>
virtualenv venv
```

Install all the dependencies for building the documentation by running;

```
cd <project-root>/doc
pip install -r requirements.txt
```

Activate the virtual environment by running;

```
source venv/bin/activate
```

Now you can build the documentation by running;

```
cd <project-root>/doc
make html
```

Finally you can open the `index.html` file at
`<project-root>/doc/build/html/index.html` to navigate the documentation.

# Build the code

To install **SystemC**, first you have to install some dependencies, by running;

```
sudo apt-get install build-essential
```

Download **SystemC 2.3** from [here](https://github.com/systemc/systemc-2.3).
After downloading, extract it. Now we need to compile and install it by running;

```
cd <extracted-dir>
./configure --prefix=/usr/local/
make
sudo make install
```

Now you can build the code by running;

```
cd <your-sc_viterbi-src-code-path>
mkdir build
cd build
cmake ..
make
```

You can find more about how to build the code
[here](http://sc-viterbi.readthedocs.io/en/latest/).

# Design/Implementation Workflow


# Project Management

To manage the development of this project we use [Taiga](http://taiga.io/).
Click [here](https://tree.taiga.io/project/pjcuadra-viterbi-encoderdecoder-algorithm-implementation/)
to see our project in Taiga.

# Issues tracking

Issues of this project are tracked using both Github's and Taiga's Issues
tracking systems.

# TODO

We are just starting so this doesn't make sense, yet!
