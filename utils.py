#!/usr/bin/env python
# encoding: utf-8

import mxnet as mx

def try_gpu():
    try:
        return mx.gpu()
    except:
        return mx.cpu()
