using COMM;
using Interfaces;
using Lumenis.RemoteServiceApi;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Support_request_app
{

    public class BaseNotifier : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
    public class RequestModel : BaseNotifier
    {
        ComLib _com = null;

        public RequestModel()
        {
            try
            {
                _com = new ComLib();
                _com._newStatusArrived += com_newStatusArrived;

            }
            catch
            {

            }
        }

        private void com_newStatusArrived(StatusEvent p_status)
        {
            try
            {
                var sessionStatusResult = p_status.Status;
                Application.Current.Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal, new Action(() =>
                {
                   
                    SessionStatus = ConvertSessionStatusEnum(sessionStatusResult);
                }));

            }
            catch (Exception ex)
            {
              
            }
        }

       
       
        public void RequestSupport()
        {
          /*if success is false it means that the RemoteService is down or that the RemoteService can't start connectwise service.
           * either way the SDK consumer can't resolve the problem and logs have to be sent.
           * 
           * */
          var success =  _com.RequestSupport();
          
           
        }

        public void StopSupport()
        {
            /*if success is false it means that the RemoteService is down or that the RemoteService can't start connectwise service.
           * either way the SDK consumer can't resolve the problem and logs have to be sent.
           * 
           * */
            var success = _com.StopSupport();
        }

        private string _sessionStatus = "None";

        public string SessionStatus { get { return _sessionStatus; } set { _sessionStatus = value; OnPropertyChanged("SessionStatus"); } }


        private string ConvertSessionStatusEnum(ScreenConnectSessionStatus p_enum)
        {
            switch (p_enum)
            {
                case ScreenConnectSessionStatus.None: return "Unknown";
                case ScreenConnectSessionStatus.CableDisconnected: return "Cable is disconnected";
                case ScreenConnectSessionStatus.SessionInStandby: return "Session in standby";
                case ScreenConnectSessionStatus.SessionIsActive: return "Session is active";
                case ScreenConnectSessionStatus.SessionDisconnected: return "Session disconnected";
                default: return "Unknown";
            }
        }
    }
   
}
