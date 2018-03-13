#include <iostream>
#include <CL/cl.hpp>

int main() {

  std::vector<cl::Platform> platforms;
  std::vector<cl::Device> devices;

  cl::Platform::get(&platforms);
  if (platforms.size() == 0){
      std::cout << " No platforms found. Check OpenCL installation!\n";
      exit(1);
  }

  int platform_index = 0, device_index;
  for (auto platform : platforms) {
    std::cout << "Platform " << platform_index << " :: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
    
    devices.clear();
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if (devices.size() == 0) {
      std::cout << "\tNo devices found.!" << std::endl;
      continue;
    }

    device_index = 0;
    for (auto device : devices) {
      std::cout<< "\tDevice  " << device_index << " :: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
      device_index++;
    }
    
    platform_index++;
  }

  std::cout << "Choose platform: ";
  std::cin >> platform_index;
  std::cout << "Choose device: ";
  std::cin >> device_index;

  cl::Platform platform = platforms[platform_index];
  platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
  cl::Device device = devices[device_index];

  platforms.clear();
  devices.clear();

  std::cout << "Chosen platform and device: " << std::endl;
  std::cout << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
  std::cout << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    
  cl::Context context({device});
  
  cl::Program::Sources sources;
  
  std::string kernel_code=
    "   void kernel simple_add(global const int* A, global const int* B, global int* C){       "
    "       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                 "
    "   }                                                                               ";
  sources.push_back({kernel_code.c_str(),kernel_code.length()});
  
  cl::Program program(context,sources);
  if(program.build({device})!=CL_SUCCESS){
    std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)<<"\n";
    exit(1);
  }
  
  
  // create buffers on the device
  cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(int)*10);
  cl::Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(int)*10);
  cl::Buffer buffer_C(context,CL_MEM_READ_WRITE,sizeof(int)*10);
  
  int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};
  
  //create queue to which we will push commands for the device.
  cl::CommandQueue queue(context,device);
  
  //write arrays A and B to the device
  queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * 10, A);
  queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 10, B);
  
  // First approach
  // cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> simple_add(cl::Kernel(program, "simple_add"));
  // cl::EnqueueArgs eargs(queue, cl::NullRange, cl::NDRange(10), cl::NullRange);
  // simple_add(eargs, buffer_A, buffer_B, buffer_C).wait();

  //alternative way to run the kernel
  cl::Kernel kernel_add = cl::Kernel(program,"simple_add");
  kernel_add.setArg(0,buffer_A);
  kernel_add.setArg(1,buffer_B);
  kernel_add.setArg(2,buffer_C);
  queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(10), cl::NullRange);
  queue.finish();
  
  int C[10];
  //read result C from the device to array C
  queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);
  
  std::cout<<" result: \n";
  for(int i=0;i<10;i++){
    std::cout<<C[i]<<" ";
  }
  std::cout << std::endl;
  
  return 0;
}