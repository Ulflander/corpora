/*jslint browser: false, node: true*/
/*global require, exports, it, describe*/


var assert = require("assert"),
    should = require('chai').should(),
    expect = require('chai').expect,
    net = require("net"),
    SOCK_PORT = 1234,
    SOCK_HOST = "127.0.0.1";

describe('Sending to service', function(){

    describe('simple query known as existing in corpus', function(){

        it('should return the corpus identifier', function(done){
            var socket = net.createConnection(SOCK_PORT, SOCK_HOST, function (){
                socket.on ('data', function (d){
                    var res = d.toString();
                    expect(res).to.equal('1,en/test');
                    done();
                });
                socket.write('en/test hello');
            });
        });
    });

    describe('simple query known as not existing in corpus', function(){

        it('should return an empty string', function(done){
            var socket = net.createConnection(SOCK_PORT, SOCK_HOST, function (){
                socket.on ('data', function (d){
                    var res = d.toString();
                    expect(res).to.equal('0');
                    done();
                });
                socket.write('en/test plop');
            });
        });
    });

    describe('Multiple corpus query known as existing in one of some corpus', function(){

        it('should return the corpus', function(done){
            var socket = net.createConnection(SOCK_PORT, SOCK_HOST, function (){
                socket.on ('data', function (d){
                    var res = d.toString();
                    expect(res).to.equal('1,en/test');
                    done();
                });
                socket.write('en/test,en/test2 hello');
            });
        });
    });

    describe('Multiple corpus query known as existing in one of some corpus', function(){

        it('should return the corpus names', function(done){
            var socket = net.createConnection(SOCK_PORT, SOCK_HOST, function (){
                socket.on ('data', function (d){
                    var res = d.toString();
                    expect(res).to.equal('2,en/test,en/test2');
                    done();
                });
                socket.write('en/test,en/test2 balloon');
            });
        });
    });



    describe('Huge corpus query with timeout set very high', function(){

        it('shouldn\'t timeout query', function(done){
            var socket = net.createConnection(SOCK_PORT, SOCK_HOST, function (){
                socket.on ('data', function (d){
                    var res = d.toString();
                    expect(res).to.equal('1,places');
                    done();
                });
                socket.setTimeout(10000);
                socket.write('places paris');
            });
        });
    });

});