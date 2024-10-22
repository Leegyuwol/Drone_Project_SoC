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
          localparam  sys_clk_freq = 100_000_000,           // �ý��� Ŭ�� ���ļ�  
          localparam pwm_freq = 50,                                // pwm ���ļ� 50hz (servo���� data sheet)
          localparam duty_step = 20*(2**regbitdepth) ,                              //128���ֱ�
          localparam temp = sys_clk_freq / pwm_freq / duty_step,       
          localparam temp_half = temp /2 )                                            
 (
    input clk,           // �Է� Ŭ�� ��ȣ
    input reset_p,       // �񵿱� ���� ��ȣ (active high)
    input [regbitdepth-1:0] motor_output,      //  �� duty�� 1~2ms ���� �Դϴ�!!
    output reg pwm
);
    wire [regbitdepth:0] duty = {1'b1, motor_output};
    reg [regbitdepth+4:0] cnt;                            // ī��Ʈ
    reg pwm_freqX128;
    
    // �� ������ pwm_freqX128 ��ȣ�� �����Ͽ�, PWM ���ļ��� 128�� ������ ����
    always @(posedge clk or posedge reset_p)begin
            if(reset_p)begin
                    pwm_freqX128 = 0;
                    cnt = 0;
            end
            else begin
                    if(cnt >= (temp - 1))cnt = 0;       // 77����  
                    else cnt = cnt + 1;                         // ī��Ʈ ����
                    
                    if(cnt < temp_half) pwm_freqX128 = 0;   // 39 ���� 0
                    else pwm_freqX128 = 1;                          // 39 �̻� 1
            end
     end
    
    wire pwm_freqX128_nedge;
    
    // pwm_freqX128 ��ȣ�� �ϰ������� �����ؼ� cnt_duty �� 1�� ����
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
             // pwm_freqX128 ��ȣ�� �ϰ������� �����ؼ� cnt_duty �� 1�� ����
            else if(pwm_freqX128_nedge)begin      
                    if(cnt_duty >= (duty_step - 1))cnt_duty = 0;
                    else cnt_duty = cnt_duty + 1;                           
                    
                    if (cnt_duty < duty)pwm = 1;        // ���� duty�����϶� pwm��  1 �ƴϸ� 0
                    else pwm = 0;
            end
     end
    
endmodule

// ���� ����� _ n(�ϰ�����)
//  cur �� old �� ��Ŭ�� �ӵ� ���̰� ���µ� �� ��¿����� �ϰ����� ������ 10ns���� �����ϴ¹��
module edge_detector_n(
    input clk,        // Ŭ�� ��ȣ �Է�
    input reset_p,    // �񵿱�� ���� ��ȣ �Է� (���� �������� Ȱ��ȭ)
    input cp,         // ���� ������ ���� Ŭ�� ��ȣ
    output p_edge,    // ��� ���� ���� ���
    output n_edge     // �ϰ� ���� ���� ���
);

    reg ff_cur, ff_old; // ����� ���� ���¸� �����ϴ� �ø��÷� ��������

    // �׻� ���: Ŭ���� �ϰ� ���� �Ǵ� ���� ��ȣ�� ��� �������� ����
    always @(negedge clk or posedge reset_p) begin
        if (reset_p) begin
            ff_cur <= 0; // ���� ��ȣ�� Ȱ��ȭ�Ǹ� ff_cur�� 0���� ����
            ff_old <= 0; // ���� ��ȣ�� Ȱ��ȭ�Ǹ� ff_old�� 0���� ����
        end else begin
            ff_old <= ff_cur; // ���� ���¸� ���� ���·� ������Ʈ
            ff_cur <= cp;     // �Է� cp�� ���� ���·� ������Ʈ
        end
    end
    
    // ���� ���� ����: ff_cur�� 1�̰� ff_old�� 0�� ��
    assign p_edge = ({ff_cur, ff_old} == 2'b10) ? 1 : 0;
    
    // ���� ���� ����: ff_cur�� 0�̰� ff_old�� 1�� ��
    assign n_edge = ({ff_cur, ff_old} == 2'b01) ? 1 : 0;

endmodule
