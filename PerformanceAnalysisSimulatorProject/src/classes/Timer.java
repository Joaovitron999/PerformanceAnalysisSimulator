/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package classes;

/**
 *
 * @author 2020.1.08.003
 */
public class Timer {
    public double currentTime;
    public double totalSimulationTime;

    public Timer(double totalSimulationTime) {
        this.currentTime = 0;
        this.totalSimulationTime = totalSimulationTime;
    }
    
    public void incrementTime(double time){
        this.currentTime += time;
    }
}
