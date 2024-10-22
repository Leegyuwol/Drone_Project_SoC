`timescale 1ns / 1ps

module tb_drone_pwm ();

    parameter regbitdepth = 8;
    
    reg clk, reset_p;
    reg [regbitdepth-1:0] motor_output;
    wire pwm;
    
    drone_bldc_motor_pwm DUT (
        clk, reset_p,
        motor_output,
        pwm
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    initial begin
        reset_p = 0;
        reset_p = 1;
        #10 reset_p = 0;
    end
    
    initial begin
        motor_output = 0;
        #30_000_000 motor_output = 8'hff;
        #30_000_000 motor_output = 0;
        #30_000_000 $finish;
    end

endmodule


module drone_bldc_motor_pwm
  #(     parameter regbitdepth = 8,     
          localparam  sys_clk_freq = 100_000_000,           // 시스템 클럭 주파수  
          localparam pwm_freq = 50,                                // pwm 주파수 50hz (servo모터 data sheet)
          localparam duty_step = 20*(2**regbitdepth) ,                              //128분주기
          localparam temp = sys_clk_freq / pwm_freq / duty_step,       
          localparam temp_half = temp /2 )                                            
 (
    input clk,           // 입력 클럭 신호
    input reset_p,       // 비동기 리셋 신호 (active high)
    input [regbitdepth-1:0] motor_output,      //  이 duty는 1~2ms 사이 입니다!!
    output reg pwm
);
    wire [regbitdepth:0] duty = {1'b1, motor_output};
    reg [regbitdepth+4:0] cnt;                            // 카운트
    reg pwm_freqX128;
    
    // 이 과정은 pwm_freqX128 신호를 생성하여, PWM 주파수를 128배 빠르게 만듬
    always @(posedge clk or posedge reset_p)begin
            if(reset_p)begin
                    pwm_freqX128 = 0;
                    cnt = 0;
            end
            else begin
                    if(cnt >= (temp - 1))cnt = 0;       // 77까지  
                    else cnt = cnt + 1;                         // 카운트 증가
                    
                    if(cnt < temp_half) pwm_freqX128 = 0;   // 39 이하 0
                    else pwm_freqX128 = 1;                          // 39 이상 1
            end
     end
    
    wire pwm_freqX128_nedge;
    
    // pwm_freqX128 신호의 하강에지를 감지해서 cnt_duty 가 1씩 증가
    edge_detector_n ed(
        .clk(clk), .reset_p(reset_p), .cp(pwm_freqX128), 
        .n_edge(pwm_freqX128_nedge)
    ); 
   
   integer cnt_duty;
   
   always @(posedge clk or posedge reset_p)begin
            if(reset_p)begin
                    cnt_duty = 0;
                    pwm = 0;
            end
             // pwm_freqX128 신호의 하강에지를 감지해서 cnt_duty 가 1씩 증가
            else if(pwm_freqX128_nedge)begin      
                    if(cnt_duty >= (duty_step - 1))cnt_duty = 0;
                    else cnt_duty = cnt_duty + 1;                           
                    
                    if (cnt_duty < duty)pwm = 1;        // 만약 duty이하일때 pwm값  1 아니면 0
                    else pwm = 0;
            end
     end
    
endmodule

// 엣지 검출기 _ n(하강엣지)
//  cur 과 old 가 한클럭 속도 차이가 나는데 그 상승엣지와 하강엣지 사이인 10ns만을 검출하는방식
module edge_detector_n(
    input clk,        // 클럭 신호 입력
    input reset_p,    // 비동기식 리셋 신호 입력 (양의 에지에서 활성화)
    input cp,         // 에지 검출을 위한 클럭 신호
    output p_edge,    // 상승 에지 검출 출력
    output n_edge     // 하강 에지 검출 출력
);

    reg ff_cur, ff_old; // 현재와 이전 상태를 저장하는 플립플롭 레지스터

    // 항상 블록: 클럭의 하강 에지 또는 리셋 신호의 상승 에지에서 실행
    always @(negedge clk or posedge reset_p) begin
        if (reset_p) begin
            ff_cur <= 0; // 리셋 신호가 활성화되면 ff_cur을 0으로 설정
            ff_old <= 0; // 리셋 신호가 활성화되면 ff_old를 0으로 설정
        end else begin
            ff_old <= ff_cur; // 현재 상태를 이전 상태로 업데이트
            ff_cur <= cp;     // 입력 cp를 현재 상태로 업데이트
        end
    end
    
    // 양의 에지 검출: ff_cur가 1이고 ff_old가 0일 때
    assign p_edge = ({ff_cur, ff_old} == 2'b10) ? 1 : 0;
    
    // 음의 에지 검출: ff_cur가 0이고 ff_old가 1일 때
    assign n_edge = ({ff_cur, ff_old} == 2'b01) ? 1 : 0;

endmodule
